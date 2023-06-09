#include "StateTestsHelper.h"
#include <retesteth/helpers/TestOutputHelper.h>
#include "Options.h"
#include "session/Session.h"
#include "testSuites/Common.h"
#include "testStructures/PrepareChainParams.h"
#include "StateTestChainRunner.h"

using namespace test;
using namespace std;
using namespace test::debug;
using namespace test::session;
using namespace test::statetests;

namespace test::statetests
{

/// Generate a blockchain test from state test filler
spDataObject FillTestAsBlockchain(StateTestInFiller const& _test)
{
    auto const allforks = _test.getAllForksFromExpectSections();
    if (hasSkipFork(allforks))
        return sDataObject(DataType::Null);

    StateTestChainRunner runner(_test);
    for (FORK const& fork : allforks)
    {
        if (runner.checkNetworkSkip(fork))
            continue;

        for (auto const& expect : _test.Expects())
        {
            if (expect.hasFork(fork))
            {
                for (auto& tr : runner.txs())
                {
                    runner.setErrorInfo(tr, fork);
                    if (!optionsAllowTransaction(tr))
                    {
                        tr.markSkipped();
                        continue;
                    }

                    // if expect section is for this transaction
                    if (!expect.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd()))
                        continue;

                    runner.prepareChainParams(fork);
                    runner.initBlockchainTestData();
                    runner.performTransactionOnExpect(tr, expect, fork);
                    runner.comparePostState(expect, fork);
                    runner.finalizeBlockchainTestData(tr, fork);

                }  // txs
            }      // if expect.count(fork)
        }          // expects
    }              // for each forks
    checkUnexecutedTransactions(runner.txs());
    return runner.getFilledTest();
}
}
