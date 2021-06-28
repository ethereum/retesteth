#include "ToolChainManager.h"
#include "ToolChainHelper.h"
#include "ToolImplHelper.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace test;

namespace toolimpl
{
ToolChainManager::ToolChainManager(SetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir)
{
    m_currentChain = 0;
    m_maxChains = 0;
    EthereumBlockState genesis(_config.genesis(), _config.state(), FH32::zero());
    m_chains[m_currentChain] = spToolChain(new ToolChain(genesis, _config, _toolPath, _tmpDir));
    m_pendingBlock =
        spEthereumBlockState(new EthereumBlockState(currentChain().lastBlock().header(), _config.state(), FH32::zero()));
    reorganizePendingBlock();
}

DataObject const ToolChainManager::mineBlocks(size_t _number, ToolChain::Mining _req)
{
    if (_number > 1)
        throw test::UpwardsException("ToolChainManager::mineBlocks number arg invalid: " + fto_string(_number));
    DataObject const res = currentChainUnsafe().mineBlock(m_pendingBlock, _req);
    reorganizePendingBlock();
    return res;
}

void ToolChainManager::rewindToBlock(VALUE const& _number)
{
    size_t number = (size_t)_number.asBigInt();
    assert(_number.asBigInt() >= 0 && _number < currentChainUnsafe().blocks().size());
    currentChainUnsafe().rewindToBlock(number);
    reorganizePendingBlock();
}

void ToolChainManager::reorganizePendingBlock()
{
    EthereumBlockState const& bl = currentChain().lastBlock();
    if (currentChain().fork() == "BerlinToLondonAt5" && bl.header()->number() == 4)
    {
        // Switch default mining to 1559 blocks
        DataObject parentData = bl.header()->asDataObject();

        VALUE newGasLimit = bl.header()->gasLimit() * ELASTICITY_MULTIPLIER;
        parentData.atKeyUnsafe("gasLimit").setString(newGasLimit.asString());

        // https://eips.ethereum.org/EIPS/eip-1559
        // INITIAL_BASE_FEE = 1000000000
        parentData["baseFeePerGas"] = VALUE(INITIAL_BASE_FEE).asString();

        spBlockHeader newPending(new BlockHeader1559(parentData));
        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(newPending, bl.state(), bl.logHash()));
    }
    else
        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(bl.header(), bl.state(), bl.logHash()));

    BlockHeader& header = m_pendingBlock.getContent().headerUnsafe().getContent();
    header.setNumber(bl.header()->number() + 1);

    // Because aleth and geth+retesteth does this, but better be empty extraData
    header.setExtraData(bl.header()->extraData());
    if (currentChain().fork() == "HomesteadToDaoAt5" && header.number() == 5)
        header.setExtraData(BYTES(DataObject("0x64616f2d686172642d666f726b")));
    header.setParentHash(currentChain().lastBlock().header()->hash());

    if (currentChain().lastBlock().header()->type() == BlockType::BlockHeader1559)
    {
        BlockHeader1559& header1559 = BlockHeader1559::castFrom(header);
        ChainOperationParams params = ChainOperationParams::defaultParams(currentChain().toolParams());
        VALUE newFee = calculateEIP1559BaseFee(params, m_pendingBlock->header(), currentChain().lastBlock().header());
        header1559.setBaseFee(VALUE(newFee));
    }
}

EthereumBlockState const& ToolChainManager::blockByNumber(VALUE const& _number) const
{
    size_t blockN = (size_t)_number.asBigInt();
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
    VALUE const& prevTime = lastBlock().header()->timestamp();
    m_pendingBlock.getContent().headerUnsafe().getContent().setTimestamp(prevTime + _time);
}

// Import Raw Block via t8ntool
FH32 ToolChainManager::importRawBlock(BYTES const& _rlp)
{
    try
    {
        // ETH_TEST_MESSAGE(_rlp.asString());
        dev::bytes decodeRLP = sfromHex(_rlp.asString());
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        toolimpl::verifyBlockRLP(rlp);

        spBlockHeader header = readBlockHeader(rlp[0]);
        ETH_TEST_MESSAGE(header->asDataObject().asJson());
        for (auto const& chain : m_chains)
            for (auto const& bl : chain.second->blocks())
                if (bl.header()->hash() == header->hash())
                    ETH_WARNING("Block with hash: `" + header->hash().asString() + "` already in chain!");

        // Check that we know the parent and prepare head to be the parentHeader of _rlp block
        reorganizeChainForParent(header->parentHash());
        verifyEthereumBlockHeader(header, currentChain());

        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(header, lastBlock().state(), FH32::zero()));
        for (auto const& trRLP : rlp[1].toList())
        {
            spTransaction spTr = readTransaction(trRLP);
            ETH_TEST_MESSAGE(spTr->asDataObject().asJson());
            addPendingTransaction(spTr);
        }

        if (rlp[2].toList().size() > 2)
            throw test::UpwardsException("Too many uncles!");

        for (auto const& unRLP : rlp[2].toList())
        {
            spBlockHeader un = readBlockHeader(unRLP);
            verifyEthereumBlockHeader(un, currentChain());
            if (un->number() >= header->number() ||
                un->number() + 7 <= header->number())
                throw test::UpwardsException("Uncle number is wrong!");
            for (auto const& pun : m_pendingBlock->uncles())
                if (pun->hash() == un->hash())
                    throw test::UpwardsException("Uncle is brother!");
            m_pendingBlock.getContent().addUncle(un);
        }

        mineBlocks(1, ToolChain::Mining::RequireValid);
        FH32 const& importedHash = lastBlock().header()->hash();
        if (importedHash != header->hash())
        {
            string errField;
            string message = "t8ntool constructed HEADER vs rawRLP HEADER: \n";
            message += compareBlockHeaders(
                lastBlock().header()->asDataObject(), header->asDataObject(), errField);
            ETH_ERROR_MESSAGE(string("Imported block hash != rawRLP hash ") + "(" + importedHash.asString() +
                              " != " + header->hash().asString() + ")" + "\n " + message);
        }

        reorganizeChainForTotalDifficulty();
        reorganizePendingBlock();
        return importedHash;
    }
    catch (std::exception const& _ex)
    {
        reorganizeChainForTotalDifficulty();
        throw test::UpwardsException(string("Error importing raw rlp block: ") + _ex.what());
    }
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


}  // namespace toolimpl
