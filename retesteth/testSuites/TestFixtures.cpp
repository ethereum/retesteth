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
static std::vector<FixtureToSuite> g_dynamic_test_suite_fixtures;

namespace
{
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

std::vector<fs::path> getSubfolders(fs::path const& _path)
{
    std::vector<fs::path> subFolders;
    using fsIterator = fs::directory_iterator;
    for (fsIterator it(_path); it != fsIterator(); ++it)
    {
        if (fs::is_directory(*it))
            subFolders.push_back(*it);
    }
    return subFolders;
}

bool hasSubfolders(fs::path const& _path)
{
    using fsIterator = fs::directory_iterator;
    for (fsIterator it(_path); it != fsIterator(); ++it)
    {
        if (fs::is_directory(*it))
            return true;
    }
    return false;
}

test_unit_id registerNewTestCase(
    vector<string>& allTestNames, FixtureToSuite const& _fixture, test_suite* _suite, string const& _caseName)
{
    auto const& suiteName = _fixture.second;
    auto const& fixture = _fixture.first;

    string const fullCaseName = string(suiteName) + "/" + _caseName;
    ETH_DC_MESSAGEC(DC::STATS2, "Registering new test case: " + fullCaseName, LogColor::YELLOW);
    allTestNames.push_back(fullCaseName);

    test_case* tcase = BOOST_TEST_CASE(boost::bind(&TestFixtureBase::execute, fixture.get()));
    tcase->p_name.value = _caseName;
    _suite->add(tcase);

    return _suite->get(_caseName);
}

void registerNewTestSuite(
    vector<string>& allTestNames, FixtureToSuite const& _fixture, test_suite* _suite, fs::path const& _path)
{
    auto const& suiteName = _fixture.second;
    auto const subFolders = getSubfolders(_path);

    string const newSuiteName = _path.stem().string();
    string const fullSuiteName = string(suiteName) + "/" + newSuiteName;
    ETH_DC_MESSAGEC(DC::STATS2, "Registering new test suite: " + fullSuiteName, LogColor::YELLOW);
    allTestNames.push_back(fullSuiteName);
    test_suite* tsuite = BOOST_TEST_SUITE(newSuiteName);

    for (auto const& path : subFolders)
    {
        auto const casename = path.stem().string();

        FixtureToSuite fixToSuite;
        auto ptr = _fixture.first.get();
        uPtrTestFixtureBase uPtr(ptr->copy());
        fixToSuite.first = std::move(uPtr);
        fixToSuite.first.get()->setAdditionalFillerFolder("/" + newSuiteName);
        fixToSuite.second = fullSuiteName;
        registerNewTestCase(allTestNames, fixToSuite, tsuite, casename);

        g_dynamic_test_suite_fixtures.push_back(std::move(fixToSuite));
    }
    _suite->add(tsuite);
}


}  // namespace


void test::DynamicTestsBoost(vector<string>& allTestNames)
{
    for (auto const& fixtureSuite : g_dynamic_test_search_fixtures)
    {
        auto const& suiteName = fixtureSuite.second;
        auto const& fixture = fixtureSuite.first;
        auto suite = getSuiteByPathName(suiteName);
        if (suite != nullptr)
        {
            auto const folder = test::getTestPath() / fixture->fillerFoler();
            fs::path const path(folder);
            if (!fs::exists(path))
                continue;
            using fsIterator = fs::directory_iterator;
            for (fsIterator it(path); it != fsIterator(); ++it)
            {
                if (fs::is_directory(*it))
                {
                    string const caseName = (*it).path().filename().string();
                    auto const caseid = suite->get(caseName);

                    if (caseid == INV_TEST_UNIT_ID && !g_exceptionNames.count(caseName))
                    {
                        if (hasSubfolders(*it))
                            registerNewTestSuite(allTestNames, fixtureSuite, suite, *it);
                        else
                            registerNewTestCase(allTestNames, fixtureSuite, suite, caseName);
                    }
                }
            }
        }
        else
            ETH_WARNING(string("test::DynamicTestsBoost failed to find suite: ") + suiteName);
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

FixtureRegistrator::FixtureRegistrator(TestFixtureBase* _fixture, string&& _suiteName)
{
    uPtrTestFixtureBase ptr(_fixture);
    auto p = std::make_pair(std::move(ptr), std::move(_suiteName));
    g_dynamic_test_search_fixtures.push_back(std::move(p));
    delete this;
}

void test::DynamicTestsBoostClean()
{
    for (auto& el : g_dynamic_test_search_fixtures)
        el.first.reset();
    g_dynamic_test_search_fixtures.clear();

    for (auto& el : g_dynamic_test_suite_fixtures)
        el.first.reset();
    g_dynamic_test_suite_fixtures.clear();
}


template <class T, class U>
void TestFixture<T, U>::_execute(std::set<TestExecution> const& _execFlags) const
{
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
    boost::filesystem::path const suiteFillerPath = m_suite.getFullPathFiller(casename).parent_path();

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

    m_suite.runAllTestsInFolder(casename);
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
template TestFixture<test::BCGeneralStateTestsShanghaiSuite,test::RequireOptionAll>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestTransitionSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestInvalidSuite,test::RequireOptionFill>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestInvalidSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::BlockchainTestValidSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);

// State link
template TestFixture<test::StateTestSuite,test::RequireOptionFill>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::StateTestSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::StateTestVMSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);
template TestFixture<test::StateTestShanghaiSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);

template TestFixture<test::TransactionTestSuite,test::DefaultFlags>::TestFixture(std::set<TestExecution> const& _execFlags);


