#include "ToolChainManager.h"
#include "ToolImplHelper.h"
#include "Verification.h"
#include <libdataobj/ConvertFile.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/Options.h>
#include <retesteth/Constants.h>
using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace test::teststruct::constnames;
namespace fs = boost::filesystem;

namespace toolimpl
{
ToolChainManager::ToolChainManager(
    spSetChainParamsArgs const& _config,
    fs::path const& _toolPath,
    fs::path const& _tmpDir,
    ToolChainGenesis _genesisPolicy)
{
    m_tmpDir = _tmpDir;
    m_toolPath = _toolPath;
    m_currentChain = 0;
    m_maxChains = 0;
    EthereumBlockState genesis(_config->genesis(), _config->state(), FH32::zero());
    m_chains[m_currentChain] = spToolChain(new ToolChain(genesis, _config, _toolPath, _tmpDir, _genesisPolicy));
    m_pendingBlock =
        spEthereumBlockState(new EthereumBlockState(currentChain().lastBlock().header(), _config->state(), FH32::zero()));
    reorganizePendingBlock();


    ETH_DC_MESSAGE(
        DC::RPC, "test_setChainParams of new block: " + BlockHeader::BlockTypeToString(lastBlock().header()->type()));
}

spDataObject const ToolChainManager::mineBlocks(size_t _number, ToolChain::Mining _req)
{
    if (_number > 1)
        throw test::UpwardsException("ToolChainManager::mineBlocks number arg invalid: " + fto_string(_number));
    const spDataObject res = currentChainUnsafe().mineBlock(m_pendingBlock, currentChainUnsafe().lastBlock(), _req);
    reorganizePendingBlock();
    return res;
}

void ToolChainManager::rewindToBlock(VALUE const& _number)
{
    const size_t number = (size_t)_number.asBigInt();
    assert(_number.asBigInt() >= 0 && _number < currentChainUnsafe().blocks().size());
    currentChainUnsafe().rewindToBlock(number);
    reorganizePendingBlock();
}

void ToolChainManager::reorganizePendingBlock()
{
    EthereumBlockState const& bl = currentChain().lastBlock();
    transitionPendingBlock(bl);

    BlockHeader& header = m_pendingBlock.getContent().headerUnsafe().getContent();
    header.setNumber(bl.header()->number() + 1);
    m_pendingBlock.getContent().setTotalDifficulty(currentChain().lastBlock().totalDifficulty());
    m_pendingBlock.getContent().clear();

    // Because aleth and geth+retesteth does this, but better be empty extraData
    header.setExtraData(bl.header()->extraData());
    if (currentChain().fork() == "HomesteadToDaoAt5" && header.number() == 5)
        header.setExtraData(BYTES(DataObject("0x64616f2d686172642d666f726b")));
    header.setParentHash(currentChain().lastBlock().header()->hash());

    if (isBlockExportBasefee(currentChain().lastBlock().header()))
    {
        BlockHeader1559& header1559 = BlockHeader1559::castFrom(header);
        const ChainOperationParams params = ChainOperationParams::defaultParams(currentChain().toolParams());
        const VALUE newFee = calculateEIP1559BaseFee(params, m_pendingBlock->header(), currentChain().lastBlock().header());
        header1559.setBaseFee(VALUE(newFee));
    }

    if (isBlockExportDifficulty(header) && Options::getCurrentConfig().cfgFile().calculateDifficulty())
    {
        const ChainOperationParams params = ChainOperationParams::defaultParams(currentChain().toolParams());
        const VALUE retestethDifficulty = calculateEthashDifficulty(params, m_pendingBlock->header(), currentChain().lastBlock().header());
        header.setDifficulty(retestethDifficulty);
    }
}

EthereumBlockState const& ToolChainManager::blockByNumber(VALUE const& _number) const
{
    size_t const blockN = (size_t)_number.asBigInt();
    if (blockN >= currentChain().blocks().size())
        throw UpwardsException(string("ToolChainManager::blockByNumer block number not found: " + _number.asDecString()));
    return currentChain().blocks().at(blockN);
}

EthereumBlockState const& ToolChainManager::blockByHash(FH32 const& _hash) const
{
    for (auto const& chain : m_chains)
    {
        for (auto const& block : chain.second->blocks())
            if (block.header()->hash() == _hash)
                return block;
    }
    throw UpwardsException(string("ToolChainManager::blockByHash block hash not found: " + _hash.asString()));
}

void ToolChainManager::modifyTimestamp(VALUE const& _time)
{
    m_pendingBlock.getContent().headerUnsafe().getContent().setTimestamp(_time);
    if (currentChain().fork() == "MergeToShanghaiAtTime15k" && m_pendingBlock->header()->timestamp() >= 15000)
        initShanghaiPendingBlock(m_pendingBlock);
    if (currentChain().fork() == "ShanghaiToCancunAtTime15k" && m_pendingBlock->header()->timestamp() >= 15000)
        initCancunPendingBlock(m_pendingBlock);
}

void ToolChainManager::reorganizeChainForParent(FH32 const& _parentHash)
{
    for (auto const& chain : m_chains)
    {
        auto const& rchain = chain.second.getCContent();
        auto const& blocks = chain.second->blocks();
        for (size_t i = 0; i < blocks.size(); i++)
        {
            if (blocks.at(i).header()->hash() == _parentHash)
            {
                if (i + 1 == blocks.size())  // last known block
                {                            // stay on this chain
                    m_currentChain = chain.first;
                    return;
                }
                else
                {
                    // clone existing chain up to this block
                    m_chains[++m_maxChains] =
                        spToolChain(new ToolChain(blocks.at(0), rchain.params(), rchain.toolPath(), rchain.tmpDir()));
                    m_currentChain = m_maxChains;
                    for (size_t j = 1; j <= i; j++)
                        m_chains[m_currentChain].getContent().insertBlock(blocks.at(j));
                    return;
                }
            }
        }
    }
    throw test::UpwardsException(string("ToolChainManager:: unknown parent hash ") + _parentHash.asString());
}

void ToolChainManager::reorganizeChainForTotalDifficulty()
{
    VALUE maxTotalDifficulty(0);
    for (auto const& chain : m_chains)
    {
        auto const& blocks = chain.second->blocks();
        auto const& lastBlock = blocks.at(blocks.size() - 1);

        if (lastBlock.totalDifficulty() > maxTotalDifficulty)
        {
            maxTotalDifficulty = lastBlock.totalDifficulty();
            m_currentChain = chain.first;
        }
    }
}

void ToolChainManager::registerWithdrawal(BYTES const& _wt)
{
    try
    {
        dev::bytes const decodeRLP = sfromHex(_wt.asString());
        dev::RLP const rlp(decodeRLP, dev::RLP::VeryStrict);
        spWithdrawal wt(new Withdrawal(rlp));
        m_pendingBlock.getContent().addWithdrawal(wt);
    }
    catch(std::exception const& _ex)
    {
        throw test::UpwardsException(string("Error importing rlp of withdrawal: ") + _ex.what());
    }
}


void ToolChainManager::init1559PendingBlock(EthereumBlockState const& _lastBlock)
{
    // Switch default mining to 1559 blocks
    spDataObject parentData = _lastBlock.header()->asDataObject();

    const VALUE newGasLimit = _lastBlock.header()->gasLimit() * ELASTICITY_MULTIPLIER;
    (*parentData).atKeyUnsafe(c_gasLimit).setString(string(newGasLimit.asString()));

    // https://eips.ethereum.org/EIPS/eip-1559
    // INITIAL_BASE_FEE = 1000000000
    (*parentData)[c_baseFeePerGas] = VALUE(INITIAL_BASE_FEE).asString();

    spBlockHeader newPending(new BlockHeader1559(parentData));
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(newPending, _lastBlock.state(), _lastBlock.logHash()));
}

bool ToolChainManager::isTerminalPoWBlock()
{
    VALUE TERMINAL_TOTAL_DIFFICULTY(DataObject("0x0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
    DataObject const& configParams = currentChain().params()->params();
    if (configParams.count("terminalTotalDifficulty"))
        TERMINAL_TOTAL_DIFFICULTY = VALUE(configParams.atKey("terminalTotalDifficulty"));

    auto const& currentChainBlocks = currentChain().blocks();
    bool parentBlockTDLessThanTerminalTD = true;
    if (currentChainBlocks.size() > 2 &&
        currentChainBlocks.at(currentChainBlocks.size() - 2).totalDifficulty() >= TERMINAL_TOTAL_DIFFICULTY)
        parentBlockTDLessThanTerminalTD = false;

    VALUE const currentTD = currentChain().lastBlock().totalDifficulty();
    // pow_block.total_difficulty >= TERMINAL_TOTAL_DIFFICULTY
    // and pow_block.parent_block.total_difficulty < TERMINAL_TOTAL_DIFFICULTY
    if (currentTD >= TERMINAL_TOTAL_DIFFICULTY && parentBlockTDLessThanTerminalTD)
        return true;
    return false;
}

void ToolChainManager::initMergePendingBlock(EthereumBlockState const& _lastBlock)
{
    // Switch default mining to Merge POS blocks
    // https://eips.ethereum.org/EIPS/eip-3675
    spDataObject parentData = _lastBlock.header()->asDataObject();
    (*parentData)[c_uncleHash] = C_EMPTY_LIST_HASH;
    (*parentData)[c_difficulty] = "0x00";
    (*parentData)[c_mixHash] = C_FH32_ZERO.asString();
    (*parentData)[c_nonce] = C_FH8_ZERO.asString();

    spBlockHeader newPending(new BlockHeaderMerge(parentData));
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(newPending, _lastBlock.state(), _lastBlock.logHash()));
}

void ToolChainManager::initShanghaiPendingBlock(EthereumBlockState const& _lastBlock)
{
    spDataObject parentData = _lastBlock.header()->asDataObject();
    (*parentData)[c_withdrawalsRoot] = C_WITHDRAWALS_EMPTY_ROOT;
    spBlockHeader newPending(new BlockHeaderShanghai(parentData));
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(newPending, _lastBlock.state(), _lastBlock.logHash()));
}

void ToolChainManager::initCancunPendingBlock(EthereumBlockState const& _lastBlock)
{
    spDataObject parentData = _lastBlock.header()->asDataObject();
    (*parentData)[c_blobGasUsed] = "0x00";
    (*parentData)[c_excessBlobGas] = "0x00";
    (*parentData)[c_parentBeaconBlockRoot] = FH32::zero().asString();
    spBlockHeader newPending(new BlockHeader4844(parentData));
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(newPending, _lastBlock.state(), _lastBlock.logHash()));
}

void ToolChainManager::transitionPendingBlock(EthereumBlockState const& _bl)
{
    auto const updatePending = [this, &_bl](){
        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(_bl.header(), _bl.state(), _bl.logHash()));
    };

    // Transform pending block to new network
    if (_bl.header()->number() == 4)
    {
        if (currentChain().fork() == "BerlinToLondonAt5")
        {
            init1559PendingBlock(_bl);
            return;
        }
    }

    if (currentChain().fork() == "MergeToShanghaiAtTime15k" && m_pendingBlock->header()->timestamp() >= 15000)
        initShanghaiPendingBlock(_bl);
    else if (currentChain().fork() == "ShanghaiToCancunAtTime15k" && m_pendingBlock->header()->timestamp() >= 15000)
        initCancunPendingBlock(_bl);
    else if (currentChain().fork() == "ArrowGlacierToMergeAtDiffC0000" && isTerminalPoWBlock())
        initMergePendingBlock(_bl);
    else
        updatePending();
}

bool ToolChainManager::isMergeChain() const
{
    assert(m_chains.size() > 0);

    auto const& defaultChain = m_chains.at(0).getCContent();
    assert(defaultChain.blocks().size() > 0);
    auto const& genesisDifficulty = defaultChain.blocks().at(0).header()->difficulty();
    return (genesisDifficulty == 0);
}

}  // namespace toolimpl
