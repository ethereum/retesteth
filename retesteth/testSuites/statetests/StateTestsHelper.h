#pragma once
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/structures.h>
#include <retesteth/testSuiteRunner/TestSuite.h>

namespace test::statetests
{

enum class Report
{
    WARNING,
    ERROR
};

extern std::string const c_trHashNotFound;
void RunTest(StateTestInFilled const& _test);
spDataObject ConvertpyTest(StateTestInFiller const& _test, TestSuite::TestSuiteOptions&);
spDataObject FillTest(StateTestInFiller const& _test, TestSuite::TestSuiteOptions&);
spDataObject FillTestAsBlockchain(StateTestInFiller const& _test, TestSuite::TestSuiteOptions&);

void checkUnexecutedTransactions(std::vector<TransactionInGeneralSection> const&, Report _report = Report::WARNING);
bool optionsAllowTransaction(TransactionInGeneralSection const& _tr);

}  // namespace test::statetests
