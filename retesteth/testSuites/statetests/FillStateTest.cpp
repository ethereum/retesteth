#include "StateTestsHelper.h"
#include "StateTestFillerRunner.h"
#include "StateTestFillerRunnerEEST.h"
#include <retesteth/helpers/TestOutputHelper.h>
#include "Options.h"
#include "session/Session.h"
#include "testSuites/Common.h"
#include "testStructures/PrepareChainParams.h"

using namespace test;
using namespace std;
using namespace test::debug;
using namespace test::session;
using namespace test::statetests;

namespace test::statetests
{
spDataObject FillTest(StateTestInFiller const& _test, TestSuite::TestSuiteOptions& _opt)
{
    auto const allforks = _test.getAllForksFromExpectSections();
    if (hasSkipFork(allforks))
        return sDataObject(DataType::Null);

    auto const& opt = Options::get();
    std::unique_ptr<StateTestFillerRunner> runner;
    if (opt.filleest)
        runner = std::make_unique<StateTestFillerRunnerEEST>(_test, _opt);
    else
        runner = std::make_unique<StateTestFillerRunner>(_test, _opt);

    if (!runner->checkBigintSkip())
    for (auto const& fork : allforks)
    {
        if (runner->checkNetworkSkip(fork))
            continue;

        runner->prepareChainParams(fork);
        for (auto const& expect : _test.Expects())
        {
            if (!expect.hasFork(fork))
                continue;

            bool expectFoundTransaction = false;
            for (auto& tr : runner->txs())
            {
                runner->setErrorInfo(tr, fork);
                bool expectChekIndexes = expect.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd());
                if (!optionsAllowTransaction(tr))
                {
                    tr.markSkipped();
                    if (expectChekIndexes)
                        expectFoundTransaction = true;
                    continue;
                }

                // if expect section is not for this transaction
                if (!expectChekIndexes)
                    continue;

                if (compareFork(fork, CMP::ge, FORK("Paris")) && _test.hasEmptyAccount())
                    ETH_ERROR_MESSAGE("Test filler pre state has empty account which is not allowed after Paris" + TestOutputHelper::get().testInfo().errorDebug());

                expectFoundTransaction = true;
                runner->performTransactionOnExpect(tr, expect, fork);
            }

            if (expectFoundTransaction == false)
            {
                ETH_ERROR_MESSAGE("Expect section does not cover any transaction: \n" + expect.initialData().asJson() +
                                  "\n" + expect.result().asDataObject()->asJson());
            }
        }

        runner->registerForkResult();
    }


    checkUnexecutedTransactions(runner->txs(), Report::ERROR);

    auto filledTest = runner->getFilledTest();
    for (auto const& test : filledTest->getSubObjects())
        verifyFilledTest(_test.unitTestVerify(), test);
    for (auto const& ex : TestOutputHelper::get().getUnitTestExceptions())
        ETH_FAIL_MESSAGE("Expected exception didn't occur: \n`" + ex + "`");
    return filledTest;
}
}
