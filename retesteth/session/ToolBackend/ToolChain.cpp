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
    m_initialParams = GCP_SPointer<SetChainParamsArgs>(_config.copy());
    m_toolParams = GCP_SPointer<ToolParams>(new ToolParams(_config.params()));

    auto const& additional = Options::getCurrentConfig().cfgFile().additionalForks();
    if (!inArray(additional, m_fork.getCContent()))
    {
        if (compareFork(m_fork, CMP::lt, FORK("London"))
            && _genesis.header()->type() == BlockType::BlockHeader1559)
        throw test::UpwardsException("Constructing 1559 genesis on network which is lower London!");
        if (compareFork(m_fork, CMP::ge, FORK("London"))
            && _genesis.header()->type() != BlockType::BlockHeader1559)
        throw test::UpwardsException("Constructing legacy genesis on network which is higher London!");
    }

    // We yet don't know the state root. ask the tool to calculate it
    ToolResponse res = mineBlockOnTool(_genesis, SealEngine::NoReward);

    EthereumBlockState genesisFixed(_genesis.header(), _genesis.state(), FH32::zero());
    genesisFixed.headerUnsafe().getContent().setStateRoot(res.stateRoot());
    genesisFixed.headerUnsafe().getContent().recalculateHash();
    genesisFixed.setTotalDifficulty(genesisFixed.header()->difficulty());
    m_blocks.push_back(genesisFixed);
}

spDataObject const ToolChain::mineBlock(EthereumBlockState const& _pendingBlock, Mining _req)
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
    VALUE toolDifficulty = calculateEthashDifficulty(params, pendingFixed.header(), lastBlock().header());
    pendingFixedHeader.setDifficulty(toolDifficulty);

    // Calculate new baseFee
    if (pendingFixedHeader.type() == BlockType::BlockHeader1559 &&
        lastBlock().header()->type() == BlockType::BlockHeader1559)
    {
        BlockHeader1559& pendingFixed1559Header = BlockHeader1559::castFrom(pendingFixedHeader);
        VALUE baseFee = calculateEIP1559BaseFee(params, pendingFixed.header(), lastBlock().header());
        pendingFixed1559Header.setBaseFee(baseFee);
    }

    // Add only those transactions which tool returned a receipt for
    // Some transactions are expected to fail. That should be detected by tests
    size_t index = 0;
    spDataObject miningResult(new DataObject());
    (*miningResult)["result"] = true;

    for (auto const& tr : _pendingBlock.transactions())
    {
        bool found = false;
        FH32 const trHash = tr->hash();
        for (auto const& trReceipt : res.receipts())
        {
            if (trReceipt.trHash() == trHash)
            {
                found = true;
                pendingFixed.addTransaction(tr);
                break;
            }
        }
        if (!found)
        {
            string const message = "t8ntool didn't return a transaction with hash: " + trHash.asString();

            // Find the rejected transaction information
            bool rejectedInfoFound = false;
            for (auto const& el : res.rejected())
            {
                if (el.index() == index)
                {
                    rejectedInfoFound = true;
                    spDataObject rejectInfo(new DataObject());
                    (*rejectInfo)["hash"] = trHash.asString();
                    (*rejectInfo)["error"] = el.error();
                    (*miningResult)["rejectedTransactions"].addArrayObject(rejectInfo);
                    break;
                }
            }
            if (!rejectedInfoFound)
                ETH_ERROR_MESSAGE("tool didn't provide information about rejected transaction");
            if (_req == Mining::AllowFailTransactions)
                ETH_WARNING_TEST(message, 6);
            else
                throw test::UpwardsException(message);
        }
        index++;
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
    if (_pendingBlock.header()->number() != pendingFixed.header()->number().asBigInt())
        throw test::UpwardsException(string("Block Number from pending block != actual chain height! (") +
                                     _pendingBlock.header()->number().asString() +
                                     " != " + pendingFixed.header()->number().asString() + ")");

    // Require new block timestamp to be > than the previous block timestamp
    if (lastBlock().header()->timestamp().asBigInt() >= pendingFixed.header()->timestamp().asBigInt())
        throw test::UpwardsException("Block Timestamp from pending block <= previous block timestamp!");

    if (_req == Mining::RequireValid)  // called on rawRLP import
    {
        if (m_fork.getContent().asString() == "HomesteadToDaoAt5" && pendingFixed.header()->number() > 4 &&
            pendingFixed.header()->number() < 19 &&
            pendingFixed.header()->extraData().asString() != "0x64616f2d686172642d666f726b")
            throw test::UpwardsException("Dao Extra Data required!");

        spDataObject const pendingH = _pendingBlock.header()->asDataObject();
        spDataObject const pendingFixedH = pendingFixed.header()->asDataObject();
        if (pendingH->asJson(0, false) != pendingFixedH->asJson(0, false))
        {
            string errField;
            string const compare = compareBlockHeaders(pendingH, pendingFixedH, errField);
            throw test::UpwardsException(string("Block from pending block != t8ntool constructed block!\n") +
                                         "Error in field: " + errField + "\n" +
                                         "rawRLP/Pending header  vs  t8ntool header \n" + compare);
        }
    }

    if (pendingFixed.header()->transactionRoot() != res.txRoot())
    {
        ETH_ERROR_MESSAGE(string("ToolChain::mineBlock txRootHash is different to one ruturned by tool \n") +
                          "constructedBlockHash: " + pendingFixed.header()->transactionRoot().asString() +
                          "\n toolTransactionRoot: " + res.txRoot().asString());
    }

    VALUE totalDifficulty(0);
    if (m_blocks.size() > 0)
        totalDifficulty = m_blocks.at(m_blocks.size() - 1).totalDifficulty();
    pendingFixed.setTotalDifficulty(totalDifficulty + pendingFixed.header()->difficulty());

    ETH_LOG("New block N: " + to_string(m_blocks.size()), 6);
    ETH_LOG("New block TD: " + totalDifficulty.asDecString() + " + " +
                pendingFixed.header()->difficulty().asDecString() + " = " +
                pendingFixed.totalDifficulty().asDecString(), 6);

    pendingFixed.setTrsTrace(res.debugTrace());
    m_blocks.push_back(pendingFixed);

    return miningResult;
}

ToolResponse ToolChain::mineBlockOnTool(EthereumBlockState const& _block, SealEngine _engine)
{
    // env.json file
    fs::path envPath = m_tmpDir / "env.json";
    BlockchainTestFillerEnv env(_block.header()->asDataObject(), m_engine);
    spDataObject envData = env.asDataObject();

    // BlockHeader hash information for tool mining
    size_t k = 0;
    for (auto const& bl : m_blocks)
        (*envData)["blockHashes"][fto_string(k++)] = bl.header()->hash().asString();
    for (auto const& un : _block.uncles())
    {
        spDataObject uncle(new DataObject());
        int delta = (int)(_block.header()->number() - un->number()).asBigInt();
        if (delta < 1)
            throw test::UpwardsException("Uncle header delta is < 1");
        (*uncle)["delta"] = delta;
        (*uncle)["address"] = un->author().asString();
        (*envData)["ommers"].addArrayObject(uncle);
    }

    // Options Hook
    Options::getCurrentConfig().performFieldReplace(envData.getContent(), FieldReplaceDir::RetestethToClient);

    string const envPathContent = envData->asJson();
    writeFile(envPath.string(), envPathContent);

    // alloc.json file
    fs::path allocPath = m_tmpDir / "alloc.json";
    string const allocPathContent = _block.state().asDataObject()->asJsonNoFirstKey();
    writeFile(allocPath.string(), allocPathContent);

    // txs.json file
    string const txsfile = _block.transactions().size() ? "txs.rlp" : "txs.json";
    fs::path txsPath = m_tmpDir / txsfile;

    dev::RLPStream txsout(_block.transactions().size());
    for (auto const& tr : _block.transactions())
        txsout.appendRaw(tr->asRLPStream().out());

    string const txsPathContent = _block.transactions().size() ?
                "\"" + dev::toString(txsout.out()) + "\"" : "[]";
    writeFile(txsPath.string(), txsPathContent);

    // output file
    fs::path outPath = m_tmpDir / "out.json";
    fs::path outAllocPath = m_tmpDir / "outAlloc.json";

    // Convert FrontierToHomesteadAt5 -> Homestead if block > 5, and get reward
    auto tupleRewardFork = prepareReward(_engine, m_fork.getContent(), _block.header()->number());

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

    bool traceCondition = Options::get().vmtrace && _block.header()->number() != 0;
    if (traceCondition)
        cmd += " --trace";

    ETH_TEST_MESSAGE("Alloc:\n" + allocPathContent);
    if (_block.transactions().size())
    {
        ETH_TEST_MESSAGE("Txs:\n" + txsPathContent);
        for (auto const& tr : _block.transactions())
            ETH_TEST_MESSAGE(tr->asDataObject()->asJson());
    }
    ETH_TEST_MESSAGE("Env:\n" + envPathContent);

    string out = test::executeCmd(cmd, ExecCMDWarning::NoWarning);

    string const outPathContent = contentsString(outPath.string());
    string const outAllocPathContent = contentsString(outAllocPath.string());
    ETH_TEST_MESSAGE("Res:\n" + outPathContent);
    ETH_TEST_MESSAGE("RAlloc:\n" + outAllocPathContent);
    ETH_TEST_MESSAGE(cmd);
    ETH_TEST_MESSAGE(out);

    if (outPathContent.empty())
        ETH_ERROR_MESSAGE("Tool returned empty file: " + outPath.string());
    if (outAllocPathContent.empty())
        ETH_ERROR_MESSAGE("Tool returned empty file: " + outAllocPath.string());

    // Construct block rpc response
    ToolResponse toolResponse(ConvertJsoncppStringToData(outPathContent));
    spDataObject returnState = ConvertJsoncppStringToData(outAllocPathContent);
    toolResponse.attachState(restoreFullState(returnState));

    if (traceCondition)
    {
        size_t i = 0;
        for (auto const& tr : _block.transactions())
        {
            fs::path txTraceFile;
            string const trNumber = test::fto_string(i++);
            txTraceFile = m_tmpDir / string("trace-" + trNumber + "-" + tr->hash().asString() + ".jsonl");
            if (fs::exists(txTraceFile))
            {
                string const preinfo =
                    "\nTransaction number: " + trNumber + ", hash: " + tr->hash().asString() + "\n";
                string const info = TestOutputHelper::get().testInfo().errorDebug();
                string const traceinfo = "\nVMTrace:" + info + cDefault + preinfo;
                toolResponse.attachDebugTrace(tr->hash(),
                    DebugVMTrace(traceinfo, trNumber, tr->hash(), contentsString(txTraceFile)));
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
