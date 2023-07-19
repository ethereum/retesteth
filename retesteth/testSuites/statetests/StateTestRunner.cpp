#include "StateTestRunner.h"
#include "StateTestsHelper.h"
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

string const c_trHashNotFound = "TR hash not found in mined block! (Check that tr is properly mined and not oog)";

StateTestRunner::StateTestRunner(StateTestInFilled const& _test)
  : m_test(_test),
    m_session(RPCSession::instance(TestOutputHelper::getThreadID()))
{
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    m_txs = buildTransactionsWithLabels();
    TestOutputHelper::get().setUnitTestExceptions(_test.unitTestExceptions());
}

std::vector<TransactionInGeneralSection> StateTestRunner::buildTransactionsWithLabels()
{
    // Recover transaction labels from filled test _info section
    std::vector<TransactionInGeneralSection> txs = m_test.GeneralTr().buildTransactions();
    for (auto const& _infoLabels : m_test.testInfo().labels())
    {
        // find a transaction with such index
        // might make sense having el.dataIndString()
        auto res = std::find_if(txs.begin(), txs.end(),
            [&_infoLabels](TransactionInGeneralSection const& el)
            { return el.dataIndS() == _infoLabels.first; });
        if (res != txs.end())
            (*res).assignTransactionLabel(":label " + _infoLabels.second);
        else
            ETH_WARNING("Test `_info` section has a label with tr.index that was not found!");
    }
    return txs;
}

bool StateTestRunner::checkBigintSkip()
{
    bool bigIntSupport = Options::getCurrentConfig().cfgFile().supportBigint();
    if (!bigIntSupport && m_test.hasBigInt())
    {
        ETH_WARNING("Skipping test that has bigint: " + m_test.testName());
        for (TransactionInGeneralSection& tr : m_txs)
            tr.markSkipped();
        return true;
    }
    return false;
}

bool StateTestRunner::checkNetworkSkip(FORK const& _network)
{
    if (networkSkip(_network, m_test.testName()))
    {
        for (TransactionInGeneralSection& tr : m_txs)
            tr.markSkipped();
        return true;
    }
    return false;
}

void StateTestRunner::prepareChainParams(FORK const& _network)
{
    TestInfo errorInfo("test_setChainParams: " + _network.asString(), m_test.testName());
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

    auto p = test::teststruct::prepareChainParams(_network, SealEngine::NoReward, m_test.Pre(), m_test.Env(), ParamsContext::StateTests);
    m_session.test_setChainParamsNoGenesis(p);
}

void StateTestRunner::setTransactionInfo(TransactionInGeneralSection& _tr, FORK const& _network)
{
    TestInfo errorInfo(_network.asString(), _tr.dataInd(), _tr.gasInd(), _tr.valueInd());
    errorInfo.setTrDataDebug(_tr.transaction()->dataLabel() + " " + _tr.transaction()->dataRawPreview() + "..");
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);
}

void StateTestRunner::performTransactionOnResult(TransactionInGeneralSection& _tr,
    StateTestPostResult const& _result, FORK const& _network)
{
    auto const& tr = _tr.transaction();
    m_session.test_modifyTimestamp(m_test.Env().firstBlockTimestamp());
    modifyTransactionChainIDByNetwork(tr, _network);
    FH32 trHash(m_session.eth_sendRawTransaction(tr->getRawBytes(), tr->getSecret()));
    _tr.assignTransactionHash(trHash);

    MineBlocksResult const mRes = m_session.test_mineBlocks(1);
    string const& testException = _result.expectException();
    compareTransactionException(tr, mRes, testException);

    VALUE latestBlockN(m_session.eth_blockNumber());
    EthGetBlockBy blockInfo(m_session.eth_getBlockByNumber(latestBlockN, Request::LESSOBJECTS));
    if (!blockInfo.hasTransaction(trHash) && testException.empty())
        ETH_ERROR_MESSAGE("StateTest::RunTest: " + c_trHashNotFound);
    _tr.markExecuted();

    // Validate post state
    FH32 const& expectedPostHash = _result.hash();
    FH32 const& remoteStateHash = blockInfo.header()->stateRoot();

    performVMTrace(_tr, remoteStateHash, _network);
    performPostState(_tr, _network, blockInfo);
    performStateDiff(_tr, _network);

    if (remoteStateHash != expectedPostHash)
    {
        ETH_DC_MESSAGE(DC::TESTLOG, "\nState Dump: \n" + getRemoteState(m_session)->asDataObject()->asJson());
        ETH_ERROR_MESSAGE("Post hash mismatch remote: " + remoteStateHash.asString() + ", expected: " + expectedPostHash.asString());
    }
    performValidations(_tr, _result);

    m_session.test_rewindToBlock(0);
    ETH_DC_MESSAGE(DC::TESTLOG, "Executed: d: " + to_string(_tr.dataInd()) + ", g: " + to_string(_tr.gasInd()) +
                                    ", v: " + to_string(_tr.valueInd()) + ", fork: " + _network.asString());
}

void StateTestRunner::performVMTrace(TransactionInGeneralSection& _tr, FH32 const& _remoteStateHash, FORK const& _network)
{
    if (Options::get().vmtrace && !Options::get().filltests)
    {
        auto const& trHash = _tr.reportedHash();
        string const testNameOut = makeFilename(_tr, _network);
        VMtraceinfo info(m_session, trHash, _remoteStateHash, testNameOut);
        printVmTrace(info);
    }
}

void StateTestRunner::performPostState(TransactionInGeneralSection& _tr, FORK const& _network, EthGetBlockBy const& _block)
{
    if (Options::get().poststate)
    {
        auto const remStateJson = getRemoteState(m_session)->asDataObject()->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning test State Dump:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + remStateJson);
        ETH_DC_MESSAGE(DC::STATE, "Reported root: " + _block.header()->stateRoot().asString());
        if (!Options::get().poststate.outpath.empty())
        {
            string const testNameOut = makeFilename(_tr, _network);
            dev::writeFile(fs::path(Options::get().poststate.outpath) / testNameOut, dev::asBytes(remStateJson));
        }
    }
}

std::string StateTestRunner::makeFilename(TransactionInGeneralSection& _tr, FORK const& _network)
{
    string testNameOut = m_test.testName();
    testNameOut += "_d";
    testNameOut += _tr.dataIndS();
    testNameOut += "g";
    testNameOut += _tr.gasIndS();
    testNameOut += "v";
    testNameOut += _tr.valueIndS();
    testNameOut += "_";
    testNameOut += _network.asString();
    testNameOut += "_";
    testNameOut += _tr.reportedHash().asString();
    testNameOut += ".txt";
    return testNameOut;
}

void StateTestRunner::performStateDiff(TransactionInGeneralSection const& _tr, FORK const& _network)
{
    auto const& opt = Options::get();
    if (opt.statediff)
    {
        if (opt.statediff.isForkSelected)
        {
            string const resKey = "d" + _tr.dataIndS() +
                                  "g" + _tr.gasIndS() +
                                  "v" + _tr.valueIndS();
            if (!m_trpostresults.count(resKey))
            {
                TrPostResults res {spState(nullptr), spState(nullptr)};
                m_trpostresults.emplace(resKey, res);
            }

            TrPostResults& results = m_trpostresults.at(resKey);
            auto& statediffA = std::get<0>(results);
            auto& statediffB = std::get<1>(results);

            if (opt.statediff.firstFork == _network.asString() && statediffA.isEmpty())
                statediffA = getRemoteState(m_session);
            if (opt.statediff.seconFork == _network.asString() && statediffB.isEmpty())
                statediffB = getRemoteState(m_session);

            if (!statediffA.isEmpty() && !statediffB.isEmpty())
            {
                auto const stateDiffJson = stateDiff(statediffA, statediffB)->asJson();
                ETH_DC_MESSAGE(DC::STATE,
                    "\nRunning test State Diff (" + opt.statediff.firstFork + " to " + opt.statediff.seconFork + "):" +
                    TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + stateDiffJson);
                statediffA.null();
                statediffB.null();
            }
        }
        else
        {
            auto const stateDiffJson = stateDiff(m_test.Pre(), getRemoteState(m_session))->asJson();
            ETH_DC_MESSAGE(DC::STATE,
                "\nRunning test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + stateDiffJson);
        }
    }
}


void StateTestRunner::performValidations(TransactionInGeneralSection& _tr, StateTestPostResult const& _result)
{
    // Validate that txbytes field has the transaction data described in test `transaction` field.
    spBYTES const& expectedBytesPtr = _result.txbytesPtr();
    if (!expectedBytesPtr.isEmpty())
    {
        auto const& tr = _tr.transaction();
        if (tr->getRawBytes().asString() != expectedBytesPtr->asString())
        {
            string const msg = string("TxBytes mismatch: test transaction section does not match txbytes in post section! ") +
                               "\n Constructed: " + expectedBytesPtr->asString() + "\n vs \n " +
                               tr->getRawBytes().asString();
            if (Options::get().chainid.initialized())
            {
                ETH_DC_MESSAGE(DC::LOWLOG, msg);
            }
            else
            {
                ETH_ERROR_MESSAGE(msg);
            }
        }
    }

    // Validate log hash
    if (Options::getDynamicOptions().getCurrentConfig().cfgFile().checkLogsHash())
    {
        FH32 const& expectedLogHash = _result.logs();
        FH32 remoteLogHash(m_session.test_getLogHash(_tr.reportedHash()));
        if (remoteLogHash != expectedLogHash)
            ETH_ERROR_MESSAGE(
                "Logs hash mismatch: '" + remoteLogHash.asString() + "', expected: '" + expectedLogHash.asString() + "'");
    }
}



void checkUnexecutedTransactions(std::vector<TransactionInGeneralSection> const& _txs, Report _report)
{
    bool atLeastOneExecuted = false;
    bool atLeastOneWithoutExpectSection = false;
    for (auto const& tr : _txs)
    {
        CHECKEXIT

        if (tr.getExecuted())
            atLeastOneExecuted = true;
        bool transactionExecutedOrSkipped = tr.getExecuted() || tr.getSkipped();
        atLeastOneWithoutExpectSection = !transactionExecutedOrSkipped || atLeastOneWithoutExpectSection;
        if (!transactionExecutedOrSkipped || atLeastOneWithoutExpectSection)
        {
            TestInfo errorInfo("all", (int)tr.dataInd(), (int)tr.gasInd(), (int)tr.valueInd());
            TestOutputHelper::get().setCurrentTestInfo(errorInfo);
            ETH_MARK_ERROR("Test has transaction uncovered with expect section!");
        }
    }
    if (!atLeastOneExecuted)
    {
        Options const& opt = Options::get();
        TestInfo errorInfo(
            opt.singleTestNet.empty() ? "N/A" : opt.singleTestNet.c_str(), opt.trData.index, opt.trGasIndex, opt.trValueIndex);
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);
    }

    if (!atLeastOneExecuted)
    {
        string const errorMessage = "Specified filter did not run a single transaction! " + TestOutputHelper::get().testInfo().errorDebug();
        if (_report == Report::ERROR)
        {
            ETH_ERROR_MESSAGE(errorMessage);
        }
        else
        {
            if (Options::isLegacy())
            {
                ETH_DC_MESSAGEC(DC::LOWLOG, errorMessage, LogColor::YELLOW);
            }
            else
            {
                ETH_WARNING(errorMessage);
            }
        }
    }
}

bool optionsAllowTransaction(TransactionInGeneralSection const& _tr)
{
    Options const& opt = Options::get();
    if ((opt.trData.index == (int)_tr.dataInd() || opt.trData.index == -1) &&
        (opt.trGasIndex == (int)_tr.gasInd() || opt.trGasIndex == -1) &&
        (opt.trValueIndex == (int)_tr.valueInd() || opt.trValueIndex == -1) &&
        (opt.trData.label == _tr.transaction()->dataLabel() || opt.trData.label.empty()))
        return true;
    return false;
}


}
