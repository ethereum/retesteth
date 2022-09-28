#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>

//Suites
#include <retesteth/testSuites/DifficultyTest.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/testSuites/TransactionTest.h>

using namespace test;

bool TestChecker::isCPUIntenseTest(std::string const& _testSuite)
{
    return test::inArray(c_cpuIntenseTests, _testSuite);
}

bool TestChecker::isTimeConsumingTest(std::string const& _testName)
{
    return test::inArray(c_timeConsumingTestSuites, _testName);
}

template <class T, class U>
TestFixture<T,U>::TestFixture(std::set<TestExecution> const& _execFlags)
{
    T suite;
    U defaultBoostFlags;
    std::set<TestExecution> allFlags = _execFlags;
    for (auto const& el : defaultBoostFlags.getFlags())
        allFlags.emplace(el);

    if (allFlags.count(TestExecution::NotRefillable) &&
        (Options::get().fillchain || Options::get().filltests))
        ETH_ERROR_MESSAGE("Tests are sealed and not refillable!");

    std::string const casename = boost::unit_test::framework::current_test_case().p_name;
    boost::filesystem::path const suiteFillerPath = suite.getFullPathFiller(casename).parent_path();

    // skip wallet test as it takes too much time (250 blocks) run it with --all flag
    if ((inArray(c_timeConsumingTestSuites, casename) || allFlags.count(TestExecution::RequireOptionAll))
        && !test::Options::get().all)
    {
        ETH_STDOUT_MESSAGE("Skipping " + casename + " because --all option is not specified.");
        test::TestOutputHelper::get().currentTestRunPP();
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
        return;
    }

    if (allFlags.count(TestExecution::RequireOptionFill) && !Options::get().filltests)
    {
        ETH_STDOUT_MESSAGE("Skipping " + casename + " because --filltests option is not specified.");
        test::TestOutputHelper::get().currentTestRunPP();
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
        return;
    }

    if (casename == "bcForgedTest")
    {
        test::TestOutputHelper::get().currentTestRunPP();
        ETH_STDOUT_MESSAGE("Skipping " + casename + " because bigint exceptions run in progress!");
        return;
    }

    suite.runAllTestsInFolder(casename);
    test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
}

// Difficulty links
template TestFixture<test::DifficultyTestSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);

// Legacy links
template TestFixture<test::LegacyConstantinopleBlockchainValidTestSuite,test::NotRefillable>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::LegacyConstantinopleBlockchainInvalidTestSuite,test::NotRefillable>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::LegacyConstantinopleBCGeneralStateTestsSuite,test::NotRefillable>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::LegacyConstantinopleStateTestSuite,test::NotRefillable>::TestFixture(std::set<TestExecution> const& _execFlags);

// BC links
template TestFixture<test::BCGeneralStateTestsSuite,test::RequireOptionAllNotRefillable>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BCGeneralStateTestsVMSuite,test::RequireOptionAll>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestTransitionSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestInvalidSuite,test::RequireOptionFill>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestInvalidSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestValidSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);

// State link
template TestFixture<test::StateTestSuite,test::RequireOptionFill>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::StateTestSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::StateTestVMSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);

template TestFixture<test::TransactionTestSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);


