#include "ToolChainHelper.h"
#include "ToolChainManager.h"
#include <Options.h>
#include <dataObject/ConvertFile.h>
#include <libdevcore/CommonIO.h>
#include <testStructures/Common.h>
#include <testStructures/types/BlockchainTests/Filler/BlockchainTestFillerEnv.h>
#include <testStructures/types/RPC/DebugVMTrace.h>
#include <testStructures/types/RPC/ToolResponse.h>

using namespace dev;
using namespace test;
using namespace teststruct;
using namespace dataobject;

namespace toolimpl
{
ToolChain::ToolChain(
    EthereumBlockState const& _genesis, SetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir)
  : m_engine(_config.sealEngine()), m_fork(new FORK(_config.params().atKey("fork"))), m_toolPath(_toolPath), m_tmpDir(_tmpDir)
{
    m_initialParams = GCP_SPointer<SetChainParamsArgs>(new SetChainParamsArgs(_config));
    m_toolParams = GCP_SPointer<ToolParams>(new ToolParams(_config.params()));

    // We yet don't know the state root. ask the tool to calculate it
    ToolResponse res = mineBlockOnTool(_genesis, SealEngine::NoReward);

    EthereumBlockState genesisFixed(_genesis.header(), _genesis.state(), FH32::zero());
    genesisFixed.headerUnsafe().getContent().setStateRoot(res.stateRoot());
    genesisFixed.headerUnsafe().getContent().recalculateHash();
    genesisFixed.addTotalDifficulty(genesisFixed.header().getCContent().difficulty());
    m_blocks.push_back(genesisFixed);
}

void ToolChain::mineBlock(EthereumBlockState const& _pendingBlock, Mining _req)
{
    ToolResponse const res = mineBlockOnTool(_pendingBlock, m_engine);

    // Pending fixed is pending header corrected by the information returned by tool
    // The tool can reject transactions changing the stateHash, TxRoot, TxReceipts, HeaderHash, GasUsed
    EthereumBlockState pendingFixed(_pendingBlock.header(), res.state(), res.logsHash());

    // Construct a block header with information that we have and what we get from t8ntool
    // The block number is current max block + 1
    BlockHeader& pendingFixedHeader = pendingFixed.headerUnsafe().getContent();
    pendingFixedHeader.setNumber(m_blocks.size());

    // Tool calculated transactions and state
    pendingFixedHeader.setStateRoot(res.stateRoot());         // Assign StateHash from the tool
    pendingFixedHeader.setGasUsed(res.totalGasUsed());        // Assign GasUsed from the tool
    pendingFixedHeader.setTransactionHash(res.txRoot());      // Assign TxRoot from the tool
    pendingFixedHeader.setTrReceiptsHash(res.receiptRoot());  // Assign TxReceipt from the tool
    pendingFixedHeader.setLogsBloom(res.logsBloom());         // Assign LogsBloom from the
    pendingFixedHeader.setStateRoot(res.stateRoot());         // Assign StateHash from the tool

    // Calculate difficulty for tool (tool does not calculate difficulty)
    ChainOperationParams params = ChainOperationParams::defaultParams(toolParams());
    u256 toolDifficulty = calculateEthashDifficulty(params, pendingFixed.header(), lastBlock().header());
    pendingFixedHeader.setDifficulty(toolDifficulty);

    // Add only those transactions which tool returned a receipt for
    // Some transactions are expected to fail. That should be detected by tests
    for (auto const& tr : _pendingBlock.transactions())
    {
        bool found = false;
        FH32 const trHash = tr.getCContent().hash();
        for (auto const& trReceipt : res.receipts())
        {
            if (trReceipt.trHash() == trHash)
            {
                found = true;
                pendingFixed.addTransaction(tr);
            }
        }
        if (!found)
        {
            string const message = "t8ntool didn't return a transaction with hash: " + trHash.asString();
            if (_req == Mining::AllowFailTransactions)
                ETH_WARNING_TEST(message, 6);
            else
                throw test::UpwardsException(message);
        }
    }

    // Treat all uncles as valid as t8ntool does not calculate uncles
    // Uncle header validity as well as RLP logic is checked before
    for (auto const& un : _pendingBlock.uncles())
    {
        verifyEthereumBlockHeader(un, *this);
        pendingFixed.addUncle(un);
    }
    pendingFixed.recalculateUncleHash(); // Rely that only uncle hash is recalculated (simulate t8ntool unclehash)

    // Calculate header hash from header fields (do not recalc tx, un hashes)
    pendingFixed.headerUnsafe().getContent().recalculateHash();

    // Blockchain rules
    verifyEthereumBlockHeader(pendingFixed.header(), *this);

    // Require number from pending block to be equal to actual block number that is imported
    if (_pendingBlock.header().getCContent().number() != pendingFixed.header().getCContent().number().asU256())
        throw test::UpwardsException(string("Block Number from pending block != actual chain height! (") +
                                     _pendingBlock.header().getCContent().number().asString() +
                                     " != " + pendingFixed.header().getCContent().number().asString() + ")");

    // Require new block timestamp to be > than the previous block timestamp
    if (lastBlock().header().getCContent().timestamp().asU256() >= pendingFixed.header().getCContent().timestamp().asU256())
        throw test::UpwardsException("Block Timestamp from pending block <= previous block timestamp!");

    if (_req == Mining::RequireValid)  // called on rawRLP import
    {
        if (m_fork.getContent().asString() == "HomesteadToDaoAt5" && pendingFixed.header().getCContent().number() > 4 &&
            pendingFixed.header().getCContent().number() < 19 &&
            pendingFixed.header().getCContent().extraData().asString() != "0x64616f2d686172642d666f726b")
            throw test::UpwardsException("Dao Extra Data required!");

        if (_pendingBlock.header().getCContent().asDataObject().asJson(0, false) !=
            pendingFixed.header().getCContent().asDataObject().asJson(0, false))
        {
            string errField;
            DataObject const pendingH = _pendingBlock.header().getCContent().asDataObject();
            DataObject const pendingFixedH = pendingFixed.header().getCContent().asDataObject();
            string const compare = compareBlockHeaders(pendingH, pendingFixedH, errField);
            throw test::UpwardsException(string("Block from pending block != t8ntool constructed block!\n") +
                                         "Error in field: " + errField + "\n" +
                                         "rawRLP/Pending header  vs  t8ntool header \n" + compare);
        }
    }

    if (pendingFixed.header().getCContent().transactionRoot() != res.txRoot())
    {
        ETH_ERROR_MESSAGE(string("ToolChain::mineBlock txRootHash is different to one ruturned by tool \n") +
                          "constructedBlockHash: " + pendingFixed.header().getCContent().transactionRoot().asString() +
                          "\n toolTransactionRoot: " + res.txRoot().asString());
    }

    VALUE totalDifficulty(0);
    if (m_blocks.size() > 0)
        totalDifficulty = m_blocks.at(m_blocks.size() - 1).totalDifficulty();
    pendingFixed.addTotalDifficulty(totalDifficulty + pendingFixed.header().getCContent().difficulty());

    ETH_LOG("New block N: " + to_string(m_blocks.size()), 6);
    ETH_LOG("New block TD: " + totalDifficulty.asDecString() + " + " +
                pendingFixed.header().getCContent().difficulty().asDecString() + " = " +
                pendingFixed.totalDifficulty().asDecString(),
        6);

    pendingFixed.setTrsTrace(res.debugTrace());
    m_blocks.push_back(pendingFixed);
}

ToolResponse ToolChain::mineBlockOnTool(EthereumBlockState const& _block, SealEngine _engine)
{
    // env.json file
    fs::path envPath = m_tmpDir / "env.json";
    BlockchainTestFillerEnv env(_block.header().getCContent().asDataObject(), m_engine);
    DataObject envData = env.asDataObject();

    // BlockHeader hash information for tool mining
    size_t k = 0;
    for (auto const& bl : m_blocks)
        envData["blockHashes"][fto_string(k++)] = bl.header().getCContent().hash().asString();
    for (auto const& un : _block.uncles())
    {
        DataObject uncle;
        int delta = (int)(_block.header().getCContent().number() - un.getCContent().number()).asU256();
        if (delta < 1)
            throw test::UpwardsException("Uncle header delta is < 1");
        uncle["delta"] = delta;
        uncle["address"] = un.getCContent().author().asString();
        envData["ommers"].addArrayObject(uncle);
    }

    writeFile(envPath.string(), envData.asJson());

    // alloc.json file
    fs::path allocPath = m_tmpDir / "alloc.json";
    writeFile(allocPath.string(), _block.state().asDataObject().asJsonNoFirstKey());

    // txs.json file
    fs::path txsPath = m_tmpDir / "txs.json";
    DataObject txs(DataType::Array);
    static u256 c_maxGasLimit = u256("0xffffffffffffffff");
    for (auto const& tr : _block.transactions())
    {
        if (tr.getCContent().gasLimit().asU256() <= c_maxGasLimit)  // tool fails on limits here.
            txs.addArrayObject(tr.getCContent().asDataObject(ExportOrder::ToolStyle));
        else
            ETH_WARNING(
                "Retesteth rejecting tx with gasLimit > 64 bits for tool" + TestOutputHelper::get().testInfo().errorDebug());
    }
    writeFile(txsPath.string(), txs.asJson());

    // output file
    fs::path outPath = m_tmpDir / "out.json";
    fs::path outAllocPath = m_tmpDir / "outAlloc.json";

    // Convert FrontierToHomesteadAt5 -> Homestead if block > 5, and get reward
    auto tupleRewardFork = prepareReward(_engine, m_fork.getContent(), _block.header().getCContent().number());

    string cmd = m_toolPath.string();
    cmd += " --input.alloc " + allocPath.string();
    cmd += " --input.txs " + txsPath.string();
    cmd += " --input.env " + envPath.string();
    cmd += " --state.fork " + std::get<1>(tupleRewardFork).asString();
    cmd += " --output.basedir " + m_tmpDir.string();
    cmd += " --output.result " + outPath.filename().string();
    cmd += " --output.alloc " + outAllocPath.filename().string();
    if (_engine != SealEngine::NoReward)
        cmd += " --state.reward " + std::get<0>(tupleRewardFork).asDecString();

    bool traceCondition = Options::get().vmtrace && _block.header().getCContent().number() != 0;
    if (traceCondition)
        cmd += " --trace";

    ETH_TEST_MESSAGE("Alloc:\n" + contentsString(allocPath.string()));
    if (_block.transactions().size())
        ETH_TEST_MESSAGE("Txs:\n" + contentsString(txsPath.string()));
    ETH_TEST_MESSAGE("Env:\n" + contentsString(envPath.string()));

    string out = test::executeCmd(cmd, ExecCMDWarning::NoWarning);

    ETH_TEST_MESSAGE("Res:\n" + contentsString(outPath.string()));
    ETH_TEST_MESSAGE("RAlloc:\n" + contentsString(outAllocPath.string()));
    ETH_TEST_MESSAGE(cmd);
    ETH_TEST_MESSAGE(out);

    // Construct block rpc response
    ToolResponse toolResponse(ConvertJsoncppStringToData(contentsString(outPath)));
    DataObject returnState = ConvertJsoncppStringToData(contentsString(outAllocPath));
    toolResponse.attachState(restoreFullState(returnState));

    if (traceCondition)
    {
        size_t i = 0;
        for (auto const& tr : _block.transactions())
        {
            fs::path txTraceFile;
            string const trNumber = test::fto_string(i++);
            txTraceFile = m_tmpDir / string("trace-" + trNumber + "-" + tr.getCContent().hash().asString() + ".jsonl");
            if (fs::exists(txTraceFile))
            {
                string const preinfo =
                    "\nTransaction number: " + trNumber + ", hash: " + tr.getCContent().hash().asString() + "\n";
                string const info = TestOutputHelper::get().testInfo().errorDebug();
                string const traceinfo = "\nVMTrace:" + info + cDefault + preinfo;
                toolResponse.attachDebugTrace(tr.getCContent().hash(),
                    DebugVMTrace(traceinfo, trNumber, tr.getCContent().hash(), contentsString(txTraceFile)));
            }
            else
                ETH_LOG("Trace file `" + txTraceFile.string() + "` not found!", 1);
        }
    }

    fs::remove(envPath);
    fs::remove(allocPath);
    fs::remove(txsPath);
    fs::remove(outPath);
    fs::remove(outAllocPath);
    return toolResponse;
}

void ToolChain::rewindToBlock(size_t _number)
{
    while (m_blocks.size() > _number + 1)
        m_blocks.pop_back();
}
}  // namespace toolimpl
