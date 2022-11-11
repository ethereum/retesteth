#include "StateTestsHelper.h"
#include "TestOutputHelper.h"
#include "Options.h"
#include "session/Session.h"
#include "testSuites/Common.h"
#include "testStructures/PrepareChainParams.h"

using namespace test;
using namespace std;
using namespace test::debug;
using namespace test::session;

namespace test::statetests
{
spDataObject FillTest(StateTestInFiller const& _test)
{
    spDataObject filledTest;
    TestOutputHelper::get().setCurrentTestName(_test.testName());

    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.Info().rawData();
    (*filledTest).atKeyPointer("env") = _test.Env().asDataObject();
    (*filledTest).atKeyPointer("pre") = _test.Pre().asDataObject();
    (*filledTest).atKeyPointer("transaction") = _test.GeneralTr().asDataObject();

    for (auto const& ex : _test.unitTestExceptions())
        (*filledTest)["exceptions"].addArrayObject(spDataObject(new DataObject(ex)));

    // Gather Transactions from general transaction section
    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();
    for (auto const& tx : txs)
    {
        // Fill up the label map to tx.data
        if (!tx.transaction()->dataLabel().empty())
        {
            static string const removePrefix = ":label ";
            string label = tx.transaction()->dataLabel();
            size_t const pos = label.find(removePrefix);
            if (pos != string::npos)
                label.erase(pos, removePrefix.length());
            if (!(*filledTest)["_info"]["labels"].count(tx.dataIndS()))
                (*filledTest)["_info"]["labels"].addSubObject(spDataObject(new DataObject(tx.dataIndS(), label)));
        }
    }

    // run transactions on all networks that we need
    for (auto const& fork : _test.getAllForksFromExpectSections())  // object constructed!!!
    {
        // Skip by --singlenet option
        bool networkSkip = false;
        Options const& opt = Options::get();

        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork) ||
            !Options::getDynamicOptions().getCurrentConfig().checkForkAllowed(fork))
            networkSkip = true;

        spDataObject forkResults;
        (*forkResults).setKey(fork.asString());

        if (!networkSkip)
        {
            auto const p = prepareChainParams(fork, SealEngine::NoReward, _test.Pre(), _test.Env(), ParamsContext::StateTests);
            session.test_setChainParams(p);
        }

        // Run transactions for defined expect sections only
        for (auto const& expect : _test.Expects())
        {
            // if expect section for this networks
            if (expect.hasFork(fork))
            {
                bool expectFoundTransaction = false;
                for (auto& tr : txs)
                {
                    TestInfo errorInfo(fork.asString(), tr.dataInd(), tr.gasInd(), tr.valueInd());
                    if (!tr.transaction()->dataLabel().empty() || !tr.transaction()->dataRawPreview().empty())
                        errorInfo.setTrDataDebug(
                            tr.transaction()->dataLabel() + " " + tr.transaction()->dataRawPreview() + "..");

                    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

                    bool expectChekIndexes = expect.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd());
                    if (!OptionsAllowTransaction(tr) || networkSkip)
                    {
                        tr.markSkipped();

                        if (expectChekIndexes)
                            expectFoundTransaction = true;
                        continue;
                    }

                    // if expect section is not for this transaction
                    if (!expectChekIndexes)
                        continue;

                    expectFoundTransaction = true;
                    session.test_modifyTimestamp(_test.Env().firstBlockTimestamp());

                    modifyTransactionChainIDByNetwork(tr.transaction(), fork);
                    FH32 trHash(session.eth_sendRawTransaction(tr.transaction()->getRawBytes(), tr.transaction()->getSecret()));

                    MineBlocksResult const mRes = session.test_mineBlocks(1);
                    string const& testException = expect.getExpectException(fork);
                    compareTransactionException(tr.transaction(), mRes, testException);

                    VALUE latestBlockN(session.eth_blockNumber());
                    EthGetBlockBy blockInfo(session.eth_getBlockByNumber(latestBlockN, Request::LESSOBJECTS));
                    if (!blockInfo.hasTransaction(trHash) && testException.empty())
                        ETH_ERROR_MESSAGE("StateTest::FillTest: " + c_trHashNotFound);
                    tr.markExecuted();

                    if (Options::get().poststate)
                        ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                                      cDefault + "Hash: " + blockInfo.header()->stateRoot().asString());

                    if (Options::get().statediff)
                    {
                        auto const stateDiffJson = stateDiff(_test.Pre(), getRemoteState(session))->asJson();
                        ETH_DC_MESSAGE(DC::STATE,
                            "\nRunning test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + stateDiffJson);
                    }

                    if (Options::get().vmtrace)
                    {
                        string const testNameOut = _test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" +
                                                   tr.valueIndS() + "_" + fork.asString() + "_" + trHash.asString() + ".txt";
                        VMtraceinfo info(session, trHash, blockInfo.header()->stateRoot(), testNameOut);
                        printVmTrace(info);
                    }

                    spDataObject transactionResults;
                    try
                    {
                        auto const remState = getRemoteState(session);
                        compareStates(expect.result(), remState);
                        if (Options::get().poststate)
                            (*transactionResults).atKeyPointer("postState") = remState.asDataObject();
                    }
                    catch (StateTooBig const&)
                    {
                        compareStates(expect.result(), session);
                    }

                    spDataObject indexes;
                    (*indexes)["data"] = tr.dataInd();
                    (*indexes)["gas"] = tr.gasInd();
                    (*indexes)["value"] = tr.valueInd();

                    (*transactionResults).atKeyPointer("indexes") = indexes;
                    (*transactionResults)["hash"] = blockInfo.header()->stateRoot().asString();
                    (*transactionResults)["txbytes"] = tr.transaction()->getRawBytes().asString();
                    if (!testException.empty())
                        (*transactionResults)["expectException"] = testException;

                    // Fill up the loghash (optional)
                    if (Options::getDynamicOptions().getCurrentConfig().cfgFile().checkLogsHash())
                    {
                        FH32 logHash(session.test_getLogHash(trHash));
                        if (!logHash.isZero())
                            (*transactionResults)["logs"] = logHash.asString();
                    }

                    (*forkResults).addArrayObject(transactionResults);
                    session.test_rewindToBlock(VALUE(0));
                }  // tx

                if (expectFoundTransaction == false)
                {
                    ETH_ERROR_MESSAGE("Expect section does not cover any transaction: \n" + expect.initialData().asJson() +
                                      "\n" + expect.result().asDataObject()->asJson());
                }
            }  // expect has fork
        }

        if (forkResults->getSubObjects().size() > 0)
            (*filledTest)["post"].addSubObject(forkResults);
    }

    checkUnexecutedTransactions(txs);
    verifyFilledTest(_test.unitTestVerify(), filledTest);
    for (auto const& ex : TestOutputHelper::get().getUnitTestExceptions())
        ETH_FAIL_MESSAGE("Expected exception didn't occur: \n`" + ex + "`");

    return filledTest;
}
}
