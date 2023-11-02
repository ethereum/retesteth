#include "StateTestsHelper.h"
#include "StateTestFillerRunner.h"
#include <retesteth/session/Session.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <libdevcore/CommonIO.h>
using namespace std;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
namespace fs = boost::filesystem;

namespace test::statetests
{

StateTestFillerRunner::StateTestFillerRunner(StateTestInFiller const& _test)
  : m_test(_test),
    m_session(RPCSession::instance(TestOutputHelper::getThreadID()))
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());

    if (m_test.hasInfo())
        (*m_filledTest).atKeyPointer("_info") = _test.Info().rawData();
    (*m_filledTest).atKeyPointer("env") = _test.Env().asDataObject();
    (*m_filledTest).atKeyPointer("pre") = _test.Pre().asDataObject();
    (*m_filledTest).atKeyPointer("transaction") = _test.GeneralTr().asDataObject();

    for (auto const& ex : _test.unitTestExceptions())
        (*m_filledTest)["exceptions"].addArrayObject(spDataObject(new DataObject(ex)));

    m_txs = _test.GeneralTr().buildTransactions();
    fillInfoWithLabels();
}

void StateTestFillerRunner::fillInfoWithLabels()
{
    for (auto const& tx : m_txs)
    {
        // Fill up the label map to tx.data
        if (!tx.transaction()->dataLabel().empty())
        {
            static string const removePrefix = ":label ";
            string label = tx.transaction()->dataLabel();
            size_t const pos = label.find(removePrefix);
            if (pos != string::npos)
                label.erase(pos, removePrefix.length());
            if (!(*m_filledTest)["_info"]["labels"].count(tx.dataIndS()))
                (*m_filledTest)["_info"]["labels"].addSubObject(spDataObject(new DataObject(tx.dataIndS(), label)));
        }
    }
}

bool StateTestFillerRunner::checkNetworkSkip(FORK const& _network)
{
    if (networkSkip(_network, m_test.testName()))
    {
        for (TransactionInGeneralSection& tr : m_txs)
            tr.markSkipped();
        return true;
    }
    return false;
}

void StateTestFillerRunner::prepareChainParams(FORK const& _network)
{
    (*m_forkResults).setKey(_network.asString());

    TestInfo errorInfo("test_setChainParams: " + _network.asString(), m_test.testName());
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

    auto const p = test::teststruct::prepareChainParams(_network, SealEngine::NoReward, m_test.Pre(), m_test.Env(), ParamsContext::StateTests);
    m_session.test_setChainParamsNoGenesis(p);
}

void StateTestFillerRunner::setErrorInfo(TransactionInGeneralSection const& _tr, FORK const& _network)
{
    auto const& ethTr = _tr.transaction();
    TestInfo errorInfo(_network.asString(), _tr.dataInd(), _tr.gasInd(), _tr.valueInd());
    if (!ethTr->dataLabel().empty() || !ethTr->dataRawPreview().empty())
        errorInfo.setTrDataDebug(ethTr->dataLabel() + " " + ethTr->dataRawPreview() + "..");
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
}

void StateTestFillerRunner::performTransactionOnExpect(TransactionInGeneralSection& _tr, StateTestFillerExpectSection const& _expect, FORK const& _network)
{
    auto const& ethTr = _tr.transaction();
    m_session.test_modifyTimestamp(m_test.Env().firstBlockTimestamp());
    modifyTransactionChainIDByNetwork(ethTr, _network);
    FH32 trHash(m_session.eth_sendRawTransaction(ethTr->getRawBytes(), ethTr->getSecret()));

    MineBlocksResult const mRes = m_session.test_mineBlocks(1);
    string const& testException = _expect.getExpectException(_network);
    compareTransactionException(ethTr, mRes, testException);

    VALUE latestBlockN(m_session.eth_blockNumber());
    EthGetBlockBy remoteBlock(m_session.eth_getBlockByNumber(latestBlockN, Request::LESSOBJECTS));
    if (!remoteBlock.hasTransaction(trHash) && testException.empty())
        ETH_ERROR_MESSAGE("StateTest::FillTest: " + c_trHashNotFound);

    _tr.markExecuted();
    _tr.assignTransactionHash(trHash);

    performPoststate(remoteBlock);
    performStatediff();
    performVmtrace(remoteBlock, _tr, _network);
    string const vmTraceStr = performVmtraceAnalys(trHash, _expect, _network);

    spDataObject transactionResults;
    try
    {
        auto const remState = getRemoteState(m_session);
        compareStates(_expect.result(), remState);
        if (Options::get().poststate)
            (*transactionResults).atKeyPointer("postState") = remState->asDataObject();
    }
    catch (StateTooBig const&)
    {
        compareStates(_expect.result(), m_session);
    }

    spDataObject indexes;
    (*indexes)["data"] = _tr.dataInd();
    (*indexes)["gas"] = _tr.gasInd();
    (*indexes)["value"] = _tr.valueInd();

    (*transactionResults).atKeyPointer("indexes") = indexes;
    (*transactionResults)["hash"] = remoteBlock.header()->stateRoot().asString();
    (*transactionResults)["txbytes"] = ethTr->getRawBytes().asString();
    if (!vmTraceStr.empty())
        (*transactionResults)["txtrace"] = "0x" + vmTraceStr;
    if (!testException.empty())
        (*transactionResults)["expectException"] = testException;

    // Fill up the loghash (optional)
    if (Options::getDynamicOptions().getCurrentConfig().cfgFile().checkLogsHash())
    {
        FH32 logHash(m_session.test_getLogHash(trHash));
        if (!logHash.isZero())
            (*transactionResults)["logs"] = logHash.asString();
    }

    m_session.test_rewindToBlock(VALUE(0));
    ETH_DC_MESSAGE(DC::TESTLOG, "Executed: d: " + to_string(_tr.dataInd()) + ", g: " + to_string(_tr.gasInd()) +
                                    ", v: " + to_string(_tr.valueInd()) + ", fork: " + _network.asString());
    (*m_forkResults).addArrayObject(transactionResults);
}


void StateTestFillerRunner::performPoststate(EthGetBlockBy const& _blockInfo)
{
    if (Options::get().poststate)
        ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                      cDefault + "Hash: " + _blockInfo.header()->stateRoot().asString());
}


void StateTestFillerRunner::performStatediff()
{
    if (Options::get().statediff)
    {
        auto const stateDiffJson = stateDiff(m_test.Pre(), getRemoteState(m_session))->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + stateDiffJson);
    }
}

void StateTestFillerRunner::performVmtrace(EthGetBlockBy const& _blockInfo, TransactionInGeneralSection const& _tr, FORK const& _network)
{
    if (Options::get().vmtrace && !Options::get().fillvmtrace)
    {
        string const testNameOut = m_test.testName() + "_d" + _tr.dataIndS() + "g" + _tr.gasIndS() + "v" +
                                   _tr.valueIndS() + "_" + _network.asString() + "_" + _tr.reportedHash().asString() + ".txt";
        VMtraceinfo info(m_session, _tr.reportedHash(), _blockInfo.header()->stateRoot(), testNameOut);
        printVmTrace(info);
    }
}

string StateTestFillerRunner::performVmtraceAnalys(FH32 const& _trHash, StateTestFillerExpectSection const& _expResult, FORK const& _network)
{
    string vmtrace;
    if (Options::get().fillvmtrace)
    {
        if (!_expResult.getExpectException(_network).empty())
            return vmtrace;
        DebugVMTrace ret(m_session.debug_traceTransaction(_trHash));
        for (auto const& log : ret.getLog())
            vmtrace += dev::toCompactHex(log.op);
    }
    return vmtrace;
}

void StateTestFillerRunner::registerForkResult()
{
    if (m_forkResults->getSubObjects().size() > 0)
        (*m_filledTest)["post"].addSubObject(m_forkResults);
    m_forkResults = spDataObject();
}

}
