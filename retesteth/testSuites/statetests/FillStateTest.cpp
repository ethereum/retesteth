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
using namespace test::statetests;

namespace  {

bool hasSkipFork(std::set<FORK> const& _allforks)
{
    Options const& opt = Options::get();
    auto const& skipforks = opt.getCurrentConfig().cfgFile().fillerSkipForks();
    for (auto const& skipfork : skipforks)
    {
        if (_allforks.count(skipfork))
        {
            ETH_WARNING(string("Test has unsupported fork `") + skipfork.asString() +
                        "` allowed to skip, skipping the test from filling!"
                        + TestOutputHelper::get().testInfo().errorDebug());
            return true;
        }
    }
    return false;
}

void fillInfoWithLabels(std::vector<TransactionInGeneralSection> const& _txs, spDataObject _filledTest)
{
    for (auto const& tx : _txs)
    {
        // Fill up the label map to tx.data
        if (!tx.transaction()->dataLabel().empty())
        {
            static string const removePrefix = ":label ";
            string label = tx.transaction()->dataLabel();
            size_t const pos = label.find(removePrefix);
            if (pos != string::npos)
                label.erase(pos, removePrefix.length());
            if (!(*_filledTest)["_info"]["labels"].count(tx.dataIndS()))
                (*_filledTest)["_info"]["labels"].addSubObject(spDataObject(new DataObject(tx.dataIndS(), label)));
        }
    }
}

void performPoststate(EthGetBlockBy const& _blockInfo)
{
    if (Options::get().poststate)
        ETH_DC_MESSAGE(DC::STATE, "PostState " + TestOutputHelper::get().testInfo().errorDebug() + " : \n" +
                                  cDefault + "Hash: " + _blockInfo.header()->stateRoot().asString());
}

void performStatediff(StateTestFillerExecInfo const& _info)
{
    if (Options::get().statediff)
    {
        auto& session = _info.session;         // Connection with the client
        auto const& test = _info.test;         // State test

        auto const stateDiffJson = stateDiff(test.Pre(), getRemoteState(session))->asJson();
        ETH_DC_MESSAGE(DC::STATE,
            "\nRunning test State Diff:" + TestOutputHelper::get().testInfo().errorDebug() + cDefault + " \n" + stateDiffJson);
    }
}

void performVmtrace(StateTestFillerExecInfo const& _info, EthGetBlockBy const& _blockInfo, FH32 const& _trHash)
{
    if (Options::get().vmtrace)
    {
        auto& session = _info.session;         // Connection with the client
        auto const& test = _info.test;         // State test
        auto& tr = _info.tr;                   // Built transaction
        auto const& fork = _info.network;      // Current network (forkname)

        string const testNameOut = test.testName() + "_d" + tr.dataIndS() + "g" + tr.gasIndS() + "v" +
                                   tr.valueIndS() + "_" + fork.asString() + "_" + _trHash.asString() + ".txt";
        VMtraceinfo info(session, _trHash, _blockInfo.header()->stateRoot(), testNameOut);
        printVmTrace(info);
    }
}

spDataObject performTransaction(StateTestFillerExecInfo const& _info)
{
    auto& session = _info.session;         // Connection with the client
    auto const& test = _info.test;         // State test
    auto& tr = _info.tr;                   // Built transaction
    auto const& fork = _info.network;      // Current network (forkname)
    auto const& expect = _info.expResult;  // Expected post state result

    session.test_modifyTimestamp(test.Env().firstBlockTimestamp());
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

    performPoststate(blockInfo);
    performStatediff(_info);
    performVmtrace(_info, blockInfo, trHash);

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

    session.test_rewindToBlock(VALUE(0));
    return transactionResults;
}

}

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

    std::vector<TransactionInGeneralSection> txs = _test.GeneralTr().buildTransactions();
    fillInfoWithLabels(txs, filledTest);

    auto const allforks = _test.getAllForksFromExpectSections();
    if (hasSkipFork(allforks))
        return spDataObject(new DataObject(DataType::Null));

    for (auto const& fork : allforks)
    {
        Options const& opt = Options::get();
        bool allowedFork = !opt.getCurrentConfig().checkForkAllowed(fork);
        if ((!opt.singleTestNet.empty() && FORK(opt.singleTestNet) != fork) || allowedFork)
        {
            for (TransactionInGeneralSection& tr : txs)
                tr.markSkipped();
            continue;
        }

        spDataObject forkResults;
        (*forkResults).setKey(fork.asString());

        auto const p = prepareChainParams(fork, SealEngine::NoReward, _test.Pre(), _test.Env(), ParamsContext::StateTests);
        session.test_setChainParams(p);

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
                    if (!OptionsAllowTransaction(tr))
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

                    auto const transactionResults = performTransaction(StateTestFillerExecInfo(session, _test, expect, tr, fork));
                    (*forkResults).addArrayObject(transactionResults);

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
