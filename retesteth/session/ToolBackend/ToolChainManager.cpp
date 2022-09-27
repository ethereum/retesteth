#include "ToolChainManager.h"
#include "ToolChainHelper.h"
#include "ToolImplHelper.h"
#include "Verification.h"
#include <libdataobj/ConvertFile.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/EthChecks.h>
#include <retesteth/FileSystem.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;

namespace toolimpl
{
ToolChainManager::ToolChainManager(spSetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir)
{
    m_tmpDir = _tmpDir;
    m_toolPath = _toolPath;
    m_currentChain = 0;
    m_maxChains = 0;
    EthereumBlockState genesis(_config->genesis(), _config->state(), FH32::zero());
    m_chains[m_currentChain] = spToolChain(new ToolChain(genesis, _config, _toolPath, _tmpDir));
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
    spDataObject const res = currentChainUnsafe().mineBlock(m_pendingBlock, currentChainUnsafe().lastBlock(), _req);
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
        init1559PendingBlock(bl);
    else if (currentChain().fork() == "ArrowGlacierToMergeAtDiffC0000" && isTerminalPoWBlock())
        initMergePendingBlock(bl);
    else
        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(bl.header(), bl.state(), bl.logHash()));

    BlockHeader& header = m_pendingBlock.getContent().headerUnsafe().getContent();
    header.setNumber(bl.header()->number() + 1);
    m_pendingBlock.getContent().setTotalDifficulty(currentChain().lastBlock().totalDifficulty());

    // Because aleth and geth+retesteth does this, but better be empty extraData
    header.setExtraData(bl.header()->extraData());
    if (currentChain().fork() == "HomesteadToDaoAt5" && header.number() == 5)
        header.setExtraData(BYTES(DataObject("0x64616f2d686172642d666f726b")));
    header.setParentHash(currentChain().lastBlock().header()->hash());

    bool isParent1559 = currentChain().lastBlock().header()->type() == BlockType::BlockHeader1559;
    bool isParentMerge = currentChain().lastBlock().header()->type() == BlockType::BlockHeaderMerge;
    if (isParent1559 || isParentMerge)
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
    m_pendingBlock.getContent().headerUnsafe().getContent().setTimestamp(_time);
}

// Import Raw Block via t8ntool
FH32 ToolChainManager::importRawBlock(BYTES const& _rlp)
{
    try
    {
        dev::bytes decodeRLP = sfromHex(_rlp.asString());
        dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
        toolimpl::verifyBlockRLP(rlp);

        spBlockHeader header = readBlockHeader(rlp[0]);
        ETH_DC_MESSAGE(DC::RPC, header->asDataObject()->asJson());
        for (auto const& chain : m_chains)
            for (auto const& bl : chain.second->blocks())
                if (bl.header()->hash() == header->hash())
                    ETH_WARNING("Block with hash: `" + header->hash().asString() + "` already in chain!");

        // Check that we know the parent and prepare head to be the parentHeader of _rlp block
        reorganizeChainForParent(header->parentHash());
        verifyEthereumBlockHeader(header, currentChain());

        m_pendingBlock = spEthereumBlockState(new EthereumBlockState(header, lastBlock().state(), FH32::zero()));
        m_pendingBlock.getContent().setTotalDifficulty(lastBlock().totalDifficulty());

        for (auto const& trRLP : rlp[1].toList())
        {
            spTransaction spTr = readTransaction(trRLP);
            ETH_DC_MESSAGE(DC::RPC, spTr->asDataObject()->asJson());
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
        FH32 const importedHash = lastBlock().header()->hash();
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

TestRawTransaction ToolChainManager::test_rawTransaction(
    BYTES const& _rlp, FORK const& _fork, fs::path const& _toolPath, fs::path const& _tmpDir)
{
    // Prepare test_mineBlocks response structure
    DataObject out;
    out["result"] = true;

    // Prepare transaction file
    fs::path txsPath = _tmpDir / "tx.rlp";

    // Rlp list header builder for given data
    test::RLPStreamU txsout(1);
    if (_rlp.firstByte() < 128)
    {
        // wrap typed transactions as RLPstring in RLPStream
        txsout.appendString(_rlp.asString());
    }
    else
        txsout.appendRaw(_rlp.asString());

    // Write data with memory allocation but faster
    writeFile(txsPath.string(), string("\"") + txsout.outHeader() + _rlp.asString().substr(2) + "\"");
    ETH_DC_MESSAGE(DC::RPC, "TXS file:\n" + string("\"") + txsout.outHeader() + _rlp.asString().substr(2) + "\"");

    string cmd = _toolPath.string();
    cmd += " --input.txs " + txsPath.string();
    cmd += " --state.fork " + _fork.asString();
    cmd += " 2>&1";
    ETH_DC_MESSAGE(DC::RPC, cmd);
    string response = test::executeCmd(cmd, ExecCMDWarning::NoWarningNoError);


    ETH_DC_MESSAGE(DC::RPC, "T9N Response:\n" + response);
    spDataObject res;
    bool errorCaught = false;

    try
    {
        res = dataobject::ConvertJsoncppStringToData(response);
    }
    catch (std::exception const& _ex) {
        if (string(_ex.what()).find("can't read json") != string::npos)
        {
            // Unable to read json. treat response as exceptional failure on wrong input
            ETH_WARNING("t9n returned invalid json, probably failed on input!");
            res = spDataObject(new DataObject(DataType::Array));
            spDataObject errObj;
            (*errObj)["error"] = response;
            (*res).addSubObject(errObj);
            ETH_DC_MESSAGE(DC::RPC, "T9N Response reconstructed:\n" + res->asJson());
            errorCaught = true;
        }
        else
            throw _ex;
    }

    string const hash = "0x" + dev::toString(dev::sha3(fromHex(_rlp.asString())));
    spDataObject tr;

    auto const& resTr = res->getSubObjects().at(0);
    if (resTr->count("intrinsicGas"))
    {
        if (resTr->atKey("intrinsicGas").type() == DataType::Integer)
            (*tr)["intrinsicGas"] = VALUE(resTr->atKey("intrinsicGas").asInt()).asString();
        else if (resTr->atKey("intrinsicGas").type() == DataType::String)
            (*tr)["intrinsicGas"] = VALUE(resTr->atKey("intrinsicGas").asString()).asString();
        else
            ETH_ERROR_MESSAGE("`intrinsicGas` field type expected to be Int or String: `" + resTr->asJson());
    }
    else
        (*tr)["intrinsicGas"] = "0x00";

    if (response.find("error") != string::npos || response.find("ERROR") != string::npos || errorCaught)
    {
        (*tr)["error"] = resTr->atKey("error").asString();
        (*tr)["sender"] = FH20::zero().asString();
        (*tr)["hash"] = hash;
        out["rejectedTransactions"].addArrayObject(tr);
    }
    else
    {
        (*tr)["sender"] = resTr->atKey("address").asString();
        (*tr)["hash"] = resTr->atKey("hash").asString();
        out["acceptedTransactions"].addArrayObject(tr);
        if (tr->atKey("hash").asString() != hash)
            ETH_ERROR_MESSAGE("t8n tool returned different tx.hash than retesteth: (t8n.hash != retesteth.hash) " + tr->atKey("hash").asString() + " != " + hash);
    }

    ETH_DC_MESSAGE(DC::RPC, "Response: test_rawTransaction `" + out.asJson());
    return TestRawTransaction(out);
}

VALUE ToolChainManager::test_calculateDifficulty(FORK const& _fork, VALUE const& _blockNumber, VALUE const& _parentTimestamp,
    VALUE const& _parentDifficulty, VALUE const& _currentTimestamp, VALUE const& _uncleNumber,
    fs::path const& _toolPath, fs::path const& _tmpDir)
{
    DifficultyStatic const& data = prepareEthereumBlockStateTemplate();

    // Constructor has serialization from data.blockA
    EthereumBlockState blockA(data.blockA, data.state, data.loghash);
    EthereumBlockState blockB(data.blockA, data.state, data.loghash);

    BlockHeader& headerA = blockA.headerUnsafe().getContent();
    headerA.setDifficulty(_parentDifficulty);
    if (_blockNumber == 0)
        ETH_ERROR_MESSAGE("ToolChainManager::test_calculateDifficulty calculating difficulty for blocknumber 0!");
    headerA.setNumber(_blockNumber - 1);
    headerA.setTimestamp(_parentTimestamp);

    // Set uncle hash to non empty
    if (_uncleNumber > 0)
        headerA.setUnclesHash(FH32("0x2dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347"));
    else
        headerA.setUnclesHash(FH32("0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347"));

    BlockHeader& headerB = blockB.headerUnsafe().getContent();
    headerB.setTimestamp(_currentTimestamp);
    headerB.setNumber(_blockNumber);
    headerB.setParentHash(headerA.hash());

    ToolChain chain(blockA, blockB, _fork, _toolPath, _tmpDir);
    return chain.lastBlock().header()->difficulty();
}


void ToolChainManager::init1559PendingBlock(EthereumBlockState const& _lastBlock)
{
    // Switch default mining to 1559 blocks
    spDataObject parentData = _lastBlock.header()->asDataObject();

    VALUE newGasLimit = _lastBlock.header()->gasLimit() * ELASTICITY_MULTIPLIER;
    (*parentData).atKeyUnsafe("gasLimit").setString(string(newGasLimit.asString()));

    // https://eips.ethereum.org/EIPS/eip-1559
    // INITIAL_BASE_FEE = 1000000000
    (*parentData)["baseFeePerGas"] = VALUE(INITIAL_BASE_FEE).asString();

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
    (*parentData)["uncleHash"] = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    (*parentData)["difficulty"] = "0x00";
    (*parentData)["mixHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
    (*parentData)["nonce"] = "0x0000000000000000";

    spBlockHeader newPending(new BlockHeaderMerge(parentData));
    m_pendingBlock = spEthereumBlockState(new EthereumBlockState(newPending, _lastBlock.state(), _lastBlock.logHash()));
}

}  // namespace toolimpl
