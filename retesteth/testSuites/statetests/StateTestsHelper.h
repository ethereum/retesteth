#pragma once
#include <retesteth/testStructures/structures.h>

namespace test::statetests
{
extern std::string const c_trHashNotFound;
bool OptionsAllowTransaction(TransactionInGeneralSection const& _tr);
void checkUnexecutedTransactions(std::vector<TransactionInGeneralSection> const& _txs);

void RunTest(StateTestInFilled const& _test);

}  // namespace test::statetests
