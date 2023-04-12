#include "StateTests.h"
#include "StateTestsHelper.h"
#include "StateTestRunner.h"
#include <retesteth/ExitHandler.h>

using namespace std;
using namespace test;
namespace test::statetests
{

void RunTest(StateTestInFilled const& _test)
{
    CHECKEXIT
    StateTestRunner runner(_test);

    for (auto const& [network, postResults] : _test.Post())
    {
        CHECKEXIT
        if (runner.checkNetworkSkip(network))
            continue;

        runner.prepareChainParams(network);
        for (StateTestPostResult const& result : postResults)
        {
            bool resultHaveCorrespondingTransaction = false;

            // look for a transaction with this indexes and execute it on a client
            for (TransactionInGeneralSection& tr : runner.txs())
            {
                CHECKEXIT

                runner.setTransactionInfo(tr, network);

                bool const checkIndexes = result.checkIndexes(tr.dataInd(), tr.gasInd(), tr.valueInd());
                if (checkIndexes)
                    resultHaveCorrespondingTransaction = true;

                if (!optionsAllowTransaction(tr))
                {
                    tr.markSkipped();
                    continue;
                }

                if (checkIndexes)
                    runner.performTransactionOnResult(tr, result, network);

            }  // ForTransactions

            ETH_ERROR_REQUIRE_MESSAGE(resultHaveCorrespondingTransaction,
                "Test `post` section has expect section without corresponding transaction!" + result.asDataObject()->asJson());
        }
    }

    checkUnexecutedTransactions(runner.txs(), Report::WARNING);

}
}  // namespace test::statetests
