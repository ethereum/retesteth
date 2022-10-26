#include "StateTestsHelper.h"
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>

using namespace std;
using namespace test::teststruct;

namespace test::statetests
{
string const c_trHashNotFound = "TR hash not found in mined block! (Check that tr is properly mined and not oog)";
bool OptionsAllowTransaction(TransactionInGeneralSection const& _tr)
{
    Options const& opt = Options::get();
    if ((opt.trData.index == (int)_tr.dataInd() || opt.trData.index == -1) &&
        (opt.trGasIndex == (int)_tr.gasInd() || opt.trGasIndex == -1) &&
        (opt.trValueIndex == (int)_tr.valueInd() || opt.trValueIndex == -1) &&
        (opt.trData.label == _tr.transaction()->dataLabel() || opt.trData.label.empty()))
        return true;
    return false;
}

void checkUnexecutedTransactions(std::vector<TransactionInGeneralSection> const& _txs)
{
    bool atLeastOneExecuted = false;
    bool atLeastOneWithoutExpectSection = false;
    for (auto const& tr : _txs)
    {
        if (ExitHandler::receivedExitSignal())
            return;
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
    ETH_ERROR_REQUIRE_MESSAGE(atLeastOneExecuted, "Specified filter did not run a single transaction! ");
}

}  // namespace test::statetests
