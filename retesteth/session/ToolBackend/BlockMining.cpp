#include "BlockMining.h"
#include "Options.h"
#include "ToolChainHelper.h"
#include "libdataobj/ConvertFile.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <testStructures/types/BlockchainTests/BlockchainTestFiller.h>
#include <filesystem>
#include <regex>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace dataobject;
using namespace test::teststruct;
namespace fs = boost::filesystem;

namespace toolimpl
{
void BlockMining::prepareEnvFile()
{
    m_envPath = m_chainRef.tmpDir() / "env.json";
    auto const& cfgFile = Options::getCurrentConfig().cfgFile();

    auto const& parentBlcokH = m_parentBlockRef.header();
    auto const& currentBlockH = m_currentBlockRef.header();

    auto spHeader = currentBlockH->asDataObject();
    spBlockchainTestFillerEnv env(readBlockchainFillerTestEnv(dataobject::move(spHeader), m_chainRef.engine()));
    spDataObject& envData = const_cast<spDataObject&>(env->asDataObject());

    if (parentBlcokH->number() != currentBlockH->number())
    {
        if (parentBlcokH->hash() != currentBlockH->parentHash())
            ETH_ERROR_MESSAGE("ToolChain::mineBlockOnTool: provided parent block != pending parent block hash!");

        if (!cfgFile.calculateDifficulty())
        {
            (*envData).removeKey("currentDifficulty");
            (*envData)["parentTimestamp"] = parentBlcokH->timestamp().asString();
            (*envData)["parentDifficulty"] = parentBlcokH->difficulty().asString();
            (*envData)["parentUncleHash"] = parentBlcokH->uncleHash().asString();
        }
    }

    if (isBlockExportCurrentRandom(currentBlockH))
        (*envData)["currentRandom"] = currentBlockH->mixHash().asString();

    if (isBlockExportWithdrawals(currentBlockH))
    {
        (*envData).atKeyPointer("withdrawals") = spDataObject(new DataObject(DataType::Array));
        for (auto const& wt : m_currentBlockRef.withdrawals())
            (*envData)["withdrawals"].addArrayObject(wt->asDataObject(ExportOrder::ToolStyle));
    }

    if (isBlockExportBasefee(parentBlcokH))
    {
        if (!cfgFile.calculateBasefee())
        {
            (*envData).removeKey("currentBaseFee");
            BlockHeader1559 const& h1559 = (BlockHeader1559 const&) parentBlcokH.getCContent();
            (*envData)["parentBaseFee"] = h1559.baseFee().asString();
            (*envData)["parentGasUsed"] = h1559.gasUsed().asString();
            (*envData)["parentGasLimit"] = h1559.gasLimit().asString();
        }
    }

    // BlockHeader hash information for tool mining
    size_t k = 0;
    for (auto const& bl : m_chainRef.blocks())
        (*envData)["blockHashes"][fto_string(k++)] = bl.header()->hash().asString();
    for (auto const& un : m_currentBlockRef.uncles())
    {
        spDataObject uncle;
        int delta = (int)(currentBlockH->number() - un->number()).asBigInt();
        if (delta < 1)
            throw test::UpwardsException("Uncle header delta is < 1");
        (*uncle)["delta"] = delta;
        (*uncle)["address"] = un->author().asString();
        (*envData)["ommers"].addArrayObject(uncle);
    }

    // Options Hook
    Options::getCurrentConfig().performFieldReplace(envData.getContent(), FieldReplaceDir::RetestethToClient);

    m_envPathContent = envData->asJson();
    writeFile(m_envPath.string(), m_envPathContent);
}

void BlockMining::prepareAllocFile()
{
    m_allocPath = m_chainRef.tmpDir() / "alloc.json";
    m_allocPathContent = m_currentBlockRef.state()->asDataObject()->asJsonNoFirstKey();
    writeFile(m_allocPath.string(), m_allocPathContent);
}

void BlockMining::prepareTxnFile()
{
    bool const exportRLP = !Options::getCurrentConfig().cfgFile().transactionsAsJson();
    string const txsfile = exportRLP ? "txs.rlp" : "txs.json";
    m_txsPath = m_chainRef.tmpDir() / txsfile;

    string txsPathContent;
    if (exportRLP)
    {
        dev::RLPStream txsout(m_currentBlockRef.transactions().size());
        for (auto const& tr : m_currentBlockRef.transactions())
            txsout.appendRaw(tr->asRLPStream().out());
        m_txsPathContent =  "\"" + dev::toString(txsout.out()) + "\"";
        writeFile(m_txsPath.string(), m_txsPathContent);
    }
    else
    {
        DataObject txs(DataType::Array);
        static u256 c_maxGasLimit = u256("0xffffffffffffffff");
        for (auto const& tr : m_currentBlockRef.transactions())
        {
            if (tr->gasLimit().asBigInt() <= c_maxGasLimit)  // tool fails on limits here.
            {
                auto trData = tr->asDataObject(ExportOrder::ToolStyle);
                (*trData)["hash"] = tr->hash().asString();

                if (tr->getSecret() != 0)
                    (*trData)["sender"] = convertSecretToPublic(tr->getSecret())->asString();

                txs.addArrayObject(trData);
            }
            else
                ETH_WARNING("Retesteth rejecting tx with gasLimit > 64 bits for tool" +
                            TestOutputHelper::get().testInfo().errorDebug());
        }
        Options::getCurrentConfig().performFieldReplace(txs, FieldReplaceDir::RetestethToClient);
        m_txsPathContent = txs.asJson();
        writeFile(m_txsPath.string(), m_txsPathContent);
    }
}

void BlockMining::executeTransition()
{
    m_outPath = m_chainRef.tmpDir() / "out.json";
    m_outAllocPath = m_chainRef.tmpDir() / "outAlloc.json";
    m_outErrorPath = m_chainRef.tmpDir() / "error.json";

    m_cmd = m_chainRef.toolPath().string();

    // Convert FrontierToHomesteadAt5 -> Homestead if block > 5, and get reward
    auto tupleRewardFork = prepareReward(m_engine, m_chainRef.fork(), m_currentBlockRef);
    m_cmd += " --state.fork " + std::get<1>(tupleRewardFork).asString();
    if (m_engine != SealEngine::NoReward)
    {
        if (m_engine == SealEngine::Genesis)
            m_cmd += " --state.reward -1";
        else
            m_cmd += " --state.reward " + std::get<0>(tupleRewardFork).asDecString();
    }

    auto const& params = m_chainRef.params().getCContent().params();
    if (params.count("chainID"))
        m_cmd += " --state.chainid " + VALUE(params.atKey("chainID")).asDecString();

    m_cmd += " --input.alloc " + m_allocPath.string();
    m_cmd += " --input.txs " + m_txsPath.string();
    m_cmd += " --input.env " + m_envPath.string();
    m_cmd += " --output.basedir " + m_chainRef.tmpDir().string();
    m_cmd += " --output.result " + m_outPath.filename().string();
    m_cmd += " --output.alloc " + m_outAllocPath.filename().string();
    m_cmd += " --output.errorlog " + m_outErrorPath.string();

    bool traceCondition = Options::get().vmtrace && m_currentBlockRef.header()->number() != 0;
    if (traceCondition)
    {
        m_cmd += " --trace ";
        if (!Options::get().vmtrace_nomemory)
            m_cmd += "--trace.memory ";
        if (!Options::get().vmtrace_noreturndata)
            m_cmd += "--trace.returndata ";
        if (Options::get().vmtrace_nostack)
            m_cmd += "--trace.nostack ";
    }

    ETH_DC_MESSAGE(DC::RPC, "Alloc:\n" + m_allocPathContent);
    if (m_currentBlockRef.transactions().size())
    {
        ETH_DC_MESSAGE(DC::RPC, "Txs:\n" + m_txsPathContent);
        for (auto const& tr : m_currentBlockRef.transactions())
            ETH_DC_MESSAGE(DC::RPC, tr->asDataObject()->asJson());
    }
    ETH_DC_MESSAGE(DC::RPC, "Env:\n" + m_envPathContent);

    int exitcode;
    string out = test::executeCmd(m_cmd, exitcode, ExecCMDWarning::NoWarningNoError);
    ETH_DC_MESSAGE(DC::RPC, m_cmd);
    if (exitcode != 0)
    {
        string const outErrorContent = dev::contentsString(m_outErrorPath.string());
        ETH_DC_MESSAGE(DC::RPC, "Err:\n" + outErrorContent);
        throw test::UpwardsException(outErrorContent.empty() ? (out.empty() ? "Tool failed: " + m_cmd : out) : outErrorContent);
    }

    ETH_DC_MESSAGE(DC::RPC, out);
}

ToolResponse BlockMining::readResult()
{
    string const outPathContent = dev::contentsString(m_outPath.string());
    string const outAllocPathContent = dev::contentsString(m_outAllocPath.string());
    ETH_DC_MESSAGE(DC::RPC, "Res:\n" + outPathContent);
    ETH_DC_MESSAGE(DC::RPC, "RAlloc:\n" + outAllocPathContent);
    if (outPathContent.empty())
        ETH_ERROR_MESSAGE("Tool returned empty file: " + m_outPath.string());
    if (outAllocPathContent.empty())
        ETH_ERROR_MESSAGE("Tool returned empty file: " + m_outAllocPath.string());

    // Construct block rpc response
    ToolResponse toolResponse(ConvertJsoncppStringToData(outPathContent));
    spDataObject returnState = ConvertJsoncppStringToData(outAllocPathContent);
    toolResponse.attachState(restoreFullState(returnState.getContent()));

    bool traceCondition = Options::get().vmtrace && m_currentBlockRef.header()->number() != 0;
    if (traceCondition)
        traceTransactions(toolResponse);

    return toolResponse;
}

void BlockMining::traceTransactions(ToolResponse& _toolResponse)
{
    size_t i = 0;
    for (auto const& tr : m_currentBlockRef.transactions())
    {
        fs::path txTraceFile;
        string const trNumber = test::fto_string(i++);
        txTraceFile = m_chainRef.tmpDir() / string("trace-" + trNumber + "-" + tr->hash().asString() + ".jsonl");
        if (fs::exists(txTraceFile))
        {
            string const preinfo = "\nTransaction number: " + trNumber + ", hash: " + tr->hash().asString() + "\n";
            string const info = TestOutputHelper::get().testInfo().errorDebug();
            string const traceinfo = "\nVMTrace:" + info + cDefault + preinfo;
            _toolResponse.attachDebugTrace(tr->hash(),
                spDebugVMTrace(new DebugVMTrace(traceinfo, txTraceFile)));
        }
        else
            ETH_DC_MESSAGE(DC::WARNING, "Trace file `" + txTraceFile.string() + "` not found!");
    }
}

BlockMining::~BlockMining()
{
    auto const& t8ntoolcall = Options::get().t8ntoolcall;
    if (!t8ntoolcall.empty())
    {
        string folder = m_chainRef.fork().asString() + "_block";
        folder += m_currentBlockRef.header()->number().asDecString() + "_";
        folder += m_currentBlockRef.header()->hash().asString().substr(0, 6);
        auto const from = m_chainRef.tmpDir().string();
        auto const to = (t8ntoolcall / fs::path(folder)).string();

        try
        {
            std::filesystem::copy(from, to);
        }
        catch (std::exception const& _ex)
        {
            ETH_WARNING(string() + "Can't export t8ntool call to destination file (check that dest is empty or use more test selectors like --singletest): \n" + _ex.what());
        }

        m_cmd = std::regex_replace(m_cmd, std::regex(m_chainRef.tmpDir().string()), to);
        fs::path const cmdFile = to + "/command.sh";
        dev::writeFile(cmdFile, dev::asBytes(m_cmd));
    }

    fs::remove(m_envPath);
    fs::remove(m_allocPath);
    fs::remove(m_outErrorPath);
    fs::remove(m_txsPath);
    fs::remove(m_outPath);
    fs::remove(m_outAllocPath);
    fs::remove_all(m_chainRef.tmpDir());
}

}  // namespace toolimpl
