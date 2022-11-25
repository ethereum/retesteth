#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/bind/bind.hpp>

//Suites
#include <retesteth/testSuites/DifficultyTest.h>
#include <retesteth/testSuites/TransactionTest.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>
#include <retesteth/testSuites/statetests/StateTests.h>

using namespace test;
using namespace test::debug;
using namespace std;
using namespace boost::unit_test;
namespace fs = boost::filesystem;

typedef std::unique_ptr<TestFixtureBase> uPtrTestFixtureBase;
typedef std::pair<uPtrTestFixtureBase, string> FixtureToSuite;
static std::vector<FixtureToSuite> g_dynamic_test_search_fixtures;

// ACCEPT HERE A SUITE NAME
FixtureRegistrator::FixtureRegistrator(TestFixtureBase* _fixture, string&& _suiteName)
{
    std::unique_ptr<TestFixtureBase> ptr(_fixture);
    g_dynamic_test_search_fixtures.push_back({std::move(ptr), std::move(_suiteName)});
}

std::set<string> g_exceptionNames = {"stExpectSection", "bcExpectSection"};
test_suite* getSuiteByPathName(std::string const& _suiteName)
{
    if (_suiteName.empty())
        return nullptr;
    auto const allSuites = test::explode(_suiteName, '/');
    test_suite* suite = &framework::master_test_suite();
    for (auto const& suiteName : allSuites)
    {
        auto const suiteid = suite->get(suiteName);
        if (suiteid != INV_TEST_UNIT_ID)
            suite = &framework::get<test_suite>(suiteid);
        else
            return nullptr;
    }
    return suite;
}

void test::DynamicTestsBoost(vector<string>& allTestNames)
{
    for (auto const& el : g_dynamic_test_search_fixtures)
    {
        auto suite = getSuiteByPathName(el.second);
        if (suite != nullptr)
        {
            auto const folder = test::getTestPath() / el.first->fillerFoler();
            fs::path const path(folder);
            using fsIterator = fs::directory_iterator;
            for (fsIterator it(path); it != fsIterator(); ++it)
            {
                if (fs::is_directory(*it))
                {
                    string const caseName = (*it).path().filename().string();
                    auto const caseid = suite->get(caseName);
                    if (caseid == INV_TEST_UNIT_ID && !g_exceptionNames.count(caseName))
                    {
                        string const fullCaseName = el.second + "/" + caseName;
                        ETH_DC_MESSAGEC(DC::STATS2, "Registering new test case: " + fullCaseName, LogColor::YELLOW);
                        allTestNames.push_back(fullCaseName);

                        test_case* tcase = BOOST_TEST_CASE(boost::bind(&TestFixtureBase::execute, el.first.get()));
                        tcase->p_name.value = caseName;
                        suite->add(tcase);
                    }
                }
            }
        }
        else
            ETH_WARNING("test::DynamicTestsBoost failed to find suite: " + el.second);
    }
}

bool TestChecker::isCPUIntenseTest(std::string const& _testSuite)
{
    return test::inArray(c_cpuIntenseTests, _testSuite);
}

bool TestChecker::isTimeConsumingTest(std::string const& _testName)
{
    return test::inArray(c_timeConsumingTestSuites, _testName);
}

template <class T, class U>
void TestFixture<T, U>::_execute(std::set<TestExecution> const& _execFlags) const
{
    T suite;
    U defaultBoostFlags;
    std::set<TestExecution> allFlags = _execFlags;
    for (auto const& el : defaultBoostFlags.getFlags())
        allFlags.emplace(el);

    if (!Options::get().forceupdate)
    {
        if (allFlags.count(TestExecution::NotRefillable) &&
            (Options::get().fillchain || Options::get().filltests))
            ETH_ERROR_MESSAGE("Tests are sealed and not refillable!");
    }

    std::string const casename = boost::unit_test::framework::current_test_case().p_name;
    boost::filesystem::path const suiteFillerPath = suite.getFullPathFiller(casename).parent_path();

    // skip wallet test as it takes too much time (250 blocks) run it with --all flag
    if ((TestChecker::isTimeConsumingTest(casename) || allFlags.count(TestExecution::RequireOptionAll))
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

template <class T, class U>
TestFixture<T, U>::TestFixture(std::set<TestExecution> const& _execFlags) : m_execFlags(_execFlags)
{
    _execute(_execFlags);
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


