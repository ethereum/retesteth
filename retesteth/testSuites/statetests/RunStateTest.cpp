#include "StateTests.h"
#include "StateTestsHelper.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testSuites/Common.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
using namespace test::statetests;
namespace fs = boost::filesystem;

namespace  {
std::vector<TransactionInGeneralSection> buildTransactionsWithLabels(StateTestInFilled const& _test)
{
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();
    // Recover transaction labels from filled test _info section
    for (auto const& _infoLabels : _test.testInfo().labels())
    {
        // find a transaction with such index
        // might make sense having el.dataIndString()
        auto res = std::find_if(txs.begin(), txs.end(),
            [&_infoLabels](TransactionInGeneralSection const& el) { return el.dataIndS() == _infoLabels.first; });
        if (res != txs.end())
            (*res).assignTransactionLabel(":label " + _infoLabels.second);
        else
            ETH_WARNING("Test `_info` section has a label with tr.index that was not found!");
    }
    return txs;
}

void performVMTrace(StateTestExecInfo const& _info, FH32 const& _trHash, FH32 const& _stateHash)
{
    if (Options::get().vmtrace && !Options::get().filltests)
    {
        auto& session = _info.session;         // Connection with the client
        auto const& test = _info.test;         // State test
        auto& tr = _info.tr;                   // Built transaction
        auto const& network = _info.network;   // Current network (forkname)


        string const testNameOut = test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" + tr.valueIndS() + "_" +
                                   network.asString() + "_" + _trHash.asString() + ".txt";
        VMtraceinfo info(session, _trHash, _stateHash, testNameOut);
        printVmTrace(info);
    }
}

void performValidations(StateTestExecInfo const& _info, FH32 const& _trHash)
{
    auto& session = _info.session;         // Connection with the client
    auto& tr = _info.tr;                   // Built transaction
    auto const& result = _info.expResult;  // Expected post state result

    // Validate that txbytes field has the transaction data described in test `transaction` field.
    spBYTES const& expectedBytesPtr = result.txbytesPtr();
    if (!expectedBytesPtr.isEmpty())
    {
        if (tr.transaction()->getRawBytes().asString() != expectedBytesPtr->asString())
        {
            string const msg = string("TxBytes mismatch: test transaction section does not match txbytes in post section! ") +
                               "\n Constructed: " + expectedBytesPtr->asString() + "\n vs \n " +
                               tr.transaction()->getRawBytes().asString();
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
        FH32 const& expectedLogHash = result.logs();
        FH32 remoteLogHash(session.test_getLogHash(_trHash));
        if (remoteLogHash != expectedLogHash)
            ETH_ERROR_MESSAGE(
                "Logs hash mismatch: '" + remoteLogHash.asString() + "', expected: '" + expectedLogHash.asString() + "'");
    }
}

void performPostState(StateTestExecInfo const& _info)
{
    if (Options::get().poststate)
    {
        auto& session = _info.session;         // Connection with the client
        auto const& test = _info.test;         // State test
        auto& tr = _info.tr;                   // Built transaction
        auto const& network = _info.network;   // Current network (forkname)

        auto const remStateJson = getRemoteState(session)->asDataObject()->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning test State Dump:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + remStateJson);
        if (!Options::get().poststate.outpath.empty())
        {
            string testNameOut = test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" + tr.valueIndS();
            testNameOut += "_" + network.asString() + ".txt";
            dev::writeFile(fs::path(Options::get().poststate.outpath) / testNameOut, dev::asBytes(remStateJson));
        }
    }
}

void performStateDiff(StateTestExecInfo const& _info)
{
    if (Options::get().statediff)
    {
        auto& session = _info.session;  // Connection with the client
        auto const stateDiffJson = stateDiff(_info.test.Pre(), getRemoteState(session))->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + stateDiffJson);
    }
}

void performTransaction(StateTestExecInfo const& _info)
{
    auto& session = _info.session;         // Connection with the client
    auto const& test = _info.test;         // State test
    auto& tr = _info.tr;                   // Built transaction
    auto const& network = _info.network;   // Current network (forkname)
    auto const& result = _info.expResult;  // Expected post state result

    session.test_modifyTimestamp(test.Env().firstBlockTimestamp());
    modifyTransactionChainIDByNetwork(tr.transaction(), network);
    FH32 trHash(session.eth_sendRawTransaction(tr.transaction()->getRawBytes(), tr.transaction()->getSecret()));

    MineBlocksResult const mRes = session.test_mineBlocks(1);
    string const& testException = result.expectException();
    compareTransactionException(tr.transaction(), mRes, testException);

    VALUE latestBlockN(session.eth_blockNumber());
    EthGetBlockBy blockInfo(session.eth_getBlockByNumber(latestBlockN, Request::LESSOBJECTS));
    if (!blockInfo.hasTransaction(trHash) && testException.empty())
        ETH_ERROR_MESSAGE("StateTest::RunTest: " + c_trHashNotFound);
    tr.markExecuted();

    // Validate post state
    FH32 const& expectedPostHash = result.hash();
    FH32 const& actualHash = blockInfo.header()->stateRoot();

    performVMTrace(_info, trHash, actualHash);
    performPostState(_info);
    performStateDiff(_info);

    if (actualHash != expectedPostHash)
    {
        ETH_DC_MESSAGE(DC::TESTLOG, "\nState Dump: \n" + getRemoteState(session)->asDataObject()->asJson());
        ETH_ERROR_MESSAGE("Post hash mismatch remote: " + actualHash.asString() + ", expected: " + expectedPostHash.asString());
    }
    performValidations(_info, trHash);

    session.test_rewindToBlock(0);
    ETH_DC_MESSAGE(DC::TESTLOG, "Executed: d: " + to_string(tr.dataInd()) + ", g: " + to_string(tr.gasInd()) +
                                    ", v: " + to_string(tr.valueInd()) + ", fork: " + network.asString());
}
}

namespace test::statetests
{
/// Read and execute the test file
void RunTest(StateTestInFilled const& _test)
{
    if (ExitHandler::receivedExitSignal())
        return;

    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    // Gather Transactions from general transaction section
    std::vector<TransactionInGeneralSection> txs = buildTransactionsWithLabels(_test);
    TestOutputHelper::get().setUnitTestExceptions(_test.unitTestExceptions());

    for (auto const& post : _test.Post())
    {
        FORK const& network = post.first;
        Options const& opt = Options::get();

        bool skipedFork = opt.getCurrentConfig().checkForkSkipOnFiller(network);
        bool allowedFork = opt.getCurrentConfig().checkForkAllowed(network);
        bool singleNetDeny = (!opt.singleTestNet.empty() && opt.singleTestNet != network.asString());
        if ( singleNetDeny || !allowedFork || skipedFork)
        {
            for (TransactionInGeneralSection& tr : txs)
                tr.markSkipped();
            if ((!allowedFork || skipedFork) && !singleNetDeny)
                ETH_WARNING("Skipping unsupported fork: " + network.asString() + " in " + _test.testName());
            continue;
        }

        TestInfo errorInfo("test_setChainParams: " + network.asString(), _test.testName());
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);

        auto p = prepareChainParams(network, SealEngine::NoReward, _test.Pre(), _test.Env(), ParamsContext::StateTests);
        session.test_setChainParamsNoGenesis(p);

        // Read all results for a specific fork
        for (StateTestPostResult const& result : post.second)
        {
            bool resultHaveCorrespondingTransaction = false;

            // look for a transaction with this indexes and execute it on a client
            for (TransactionInGeneralSection& tr : txs)
            {
                if (ExitHandler::receivedExitSignal())
                    return;

                TestInfo errorInfo(network.asString(), tr.dataInd(), tr.gasInd(), tr.valueInd());
                errorInfo.setTrDataDebug(tr.transaction()->dataLabel() + " " + tr.transaction()->dataRawPreview() + "..");

                TestOutputHelper::get().setCurrentTestInfo(errorInfo);
                bool checkIndexes = result.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd());
                if (checkIndexes)
                    resultHaveCorrespondingTransaction = true;

                if (!OptionsAllowTransaction(tr))
                {
                    tr.markSkipped();
                    continue;
                }

                if (checkIndexes)
                    performTransaction(StateTestExecInfo(session, _test, result, tr, network));

            }  // ForTransactions

            ETH_ERROR_REQUIRE_MESSAGE(resultHaveCorrespondingTransaction,
                "Test `post` section has expect section without corresponding transaction!" + result.asDataObject()->asJson());
        }
    }

    checkUnexecutedTransactions(txs, Report::WARNING);
}
}  // namespace test::statetests
