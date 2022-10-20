#include "StateTests.h"
#include "StateTestsHelper.h"
#include <libdevcore/CommonIO.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/testSuites/Common.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
namespace fs = boost::filesystem;

namespace  {
std::vector<TransactionInGeneralSection> recoverTransactionLabels(StateTestInFilled const& _test)
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
    std::vector<TransactionInGeneralSection> txs = recoverTransactionLabels(_test);

    bool forkNotAllowed = false;
    for (auto const& post : _test.Post())
    {
        bool networkSkip = false;
        FORK const& network = post.first;
        Options const& opt = Options::get();

        // If options singlenet select different network or test has network that is not allowed by clinet configs
        if (!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != network)
            networkSkip = true;
        else if (!Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(network))
        {
            networkSkip = true;
            forkNotAllowed = true;
            ETH_WARNING("Skipping unsupported fork: " + network.asString() + " in " + _test.testName());
        }
        else
        {
            auto p = prepareChainParams(network, SealEngine::NoReward, _test.Pre(), _test.Env(), ParamsContext::StateTests);
            session.test_setChainParams(p);
        }

        // One test could have many transactions on same chainParams
        // It is expected that for a setted chainParams there going to be a transaction
        // Rather then all transactions would be filtered out and not executed at all

        // read all results for a specific fork
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

                if (!OptionsAllowTransaction(tr) || networkSkip)
                {
                    tr.markSkipped();
                    continue;
                }

                if (checkIndexes)
                {
                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp());
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
                    if (Options::get().vmtrace && !Options::get().filltests)
                    {
                        string const testNameOut = _test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" +
                                                   tr.valueIndS() + "_" + network.asString() + "_" + trHash.asString() + ".txt";
                        VMtraceinfo info(session, trHash, blockInfo.header()->stateRoot(), testNameOut);
                        printVmTrace(info);
                    }

                    FH32 const& actualHash = blockInfo.header()->stateRoot();
                    if (actualHash != expectedPostHash)
                    {
                        ETH_DC_MESSAGE(DC::TESTLOG, "\nState Dump: \n" + getRemoteState(session).asDataObject()->asJson());
                        ETH_ERROR_MESSAGE("Post hash mismatch remote: " + actualHash.asString() +
                                          ", expected: " + expectedPostHash.asString());
                    }
                    if (Options::get().poststate)
                    {
                        auto const remStateJson = getRemoteState(session).asDataObject()->asJson();
                        ETH_DC_MESSAGE(
                            DC::STATE, "\nRunning test State Dump:" + TestOutputHelper::get().testInfo().errorDebug() +
                                           cDefault + " \n" + remStateJson);
                        if (!Options::get().poststate.outpath.empty())
                        {
                            string testNameOut =
                                _test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" + tr.valueIndS();
                            testNameOut += "_" + network.asString() + ".txt";
                            dev::writeFile(
                                fs::path(Options::get().poststate.outpath) / testNameOut, dev::asBytes(remStateJson));
                        }
                    }

                    // Validate that txbytes field has the transaction data described in test `transaction` field.
                    spBYTES const& expectedBytesPtr = result.bytesPtr();
                    if (!expectedBytesPtr.isEmpty())
                    {
                        if (tr.transaction()->getRawBytes().asString() != expectedBytesPtr->asString())
                            ETH_ERROR_MESSAGE(
                                "TxBytes mismatch: test transaction section doest not match txbytes in post section!");
                    }

                    // Validate log hash
                    if (Options::getDynamicOptions().getCurrentConfig().cfgFile().checkLogsHash())
                    {
                        FH32 const& expectedLogHash = result.logs();
                        FH32 remoteLogHash(session.test_getLogHash(trHash));
                        if (remoteLogHash != expectedLogHash)
                            ETH_ERROR_MESSAGE("Logs hash mismatch: '" + remoteLogHash.asString() + "', expected: '" +
                                              expectedLogHash.asString() + "'");
                    }

                    session.test_rewindToBlock(0);
                    ETH_DC_MESSAGE(DC::TESTLOG, "Executed: d: " + to_string(tr.dataInd()) + ", g: " + to_string(tr.gasInd()) +
                                                    ", v: " + to_string(tr.valueInd()) + ", fork: " + network.asString());
                }
            }  // ForTransactions

            ETH_ERROR_REQUIRE_MESSAGE(resultHaveCorrespondingTransaction,
                "Test `post` section has expect section without corresponding transaction!" + result.asDataObject()->asJson());
        }
    }

    if (!forkNotAllowed)
        checkUnexecutedTransactions(txs);
}
}  // namespace test::statetests
