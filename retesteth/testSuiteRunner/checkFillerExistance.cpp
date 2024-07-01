#include "EthChecks.h"
#include "Options.h"
#include <retesteth/helpers/TestHelper.h>
#include "TestSuite.h"
#include "TestSuiteHelperFunctions.h"
#include <retesteth/helpers/TestOutputHelper.h>

#include <libdevcore/CommonIO.h>


using namespace std;
using namespace test;
using namespace test::debug;
using namespace test::testsuite;
namespace fs = boost::filesystem;

namespace
{
string const getTestNameFilter()
{
    test::Options const& opt = test::Options::get();
    string const testNameFilter = opt.singletest.name.empty() ? string() : opt.singletest.name;
    string filter = testNameFilter;
    filter += opt.singleTestNet.empty() ? string() : " " + opt.singleTestNet;
    filter += opt.getGStateTransactionFilter();
    ETH_DC_MESSAGE(
        DC::TESTLOG, "Checking test filler hashes for " + boost::unit_test::framework::current_test_case().full_name());
    if (!filter.empty())
        ETH_DC_MESSAGE(DC::STATS, "Filter: '" + filter + "'");
    return testNameFilter;
}

TestSuite::AbsoluteFilledTestPath createPathIfNotExist(TestSuite::AbsoluteFilledTestPath const& _path)
{
    if (!fs::exists(_path.path()) && Options::get().filltests)
    {
        ETH_DC_MESSAGE(
            DC::WARNING, "Tests folder does not exists, creating test folder: '" + string(_path.path().c_str()) + "'");
        fs::create_directories(_path.path());
    }
    return _path;
}
}  // namespace

namespace test
{

void checkGeneratedTest(fs::path const& _filledPath, std::vector<fs::path> const& _fillers,
    std::vector<fs::path>& _outdatedTestFillers,
    std::vector<fs::path>& _verifiedGeneratedTests)
{
    auto const& opt = Options::get();
    string message = "Tests are not generated (forgot --filltests?): ";
    for (auto const& filler : _fillers)
    {
        TestInfo errorInfo("CheckFiller", filler.stem().string());
        TestOutputHelper::get().setCurrentTestInfo(errorInfo);

        bool outdatedFillerRegistered = false;
        vector<string> generatedTestNames = getGeneratedTestNames(filler);
        for (auto const& testName : generatedTestNames)
        {
            fs::path generatedTestPath = _filledPath / (testName + ".json");
            if (fs::exists(generatedTestPath))
            {
                // if --filltests is set, mark all tests as outdated
                if ((opt.filltests && !opt.filloutdated) || checkFillerHash(generatedTestPath, filler))
                {
                    if (!opt.filloutdated)
                        message += "\n " + filler.string() + " => " + generatedTestPath.string();
                    if (!outdatedFillerRegistered)
                    {
                        _outdatedTestFillers.emplace_back(filler);
                        outdatedFillerRegistered = true;
                    }
                }
                _verifiedGeneratedTests.emplace_back(generatedTestPath);
            }
            else
            {
                if (!opt.filloutdated)
                    message += "\n " + filler.string() + " => " + generatedTestPath.string();
                if (!outdatedFillerRegistered)
                {
                    _outdatedTestFillers.emplace_back(filler);
                    outdatedFillerRegistered = true;
                }
            }
        }
    }

    if (!opt.filloutdated && _outdatedTestFillers.size() > 0 && !opt.filltests)
    {
        message += "\n";
        ETH_ERROR_MESSAGE(message);
    }
}

void checkFillersNotWithTheSameName(std::vector<fs::path> const& _allTestFillers, string const& _testNameFilter)
{
    if (!_testNameFilter.empty() && _allTestFillers.size() == 0)
        ETH_ERROR_MESSAGE("Could not find a filler for provided --singletest filter: '" + _testNameFilter + "'");

    std::set<string> fillerNames;
    std::vector<fs::path> fillersWithSameName;
    for (auto const& test : _allTestFillers)
    {
        if (fillerNames.count(test.stem().string()))
            fillersWithSameName.emplace_back(test);
        fillerNames.emplace(test.stem().string());
    }

    if (fillersWithSameName.size() > 0)
    {
        string ambiguousTests;
        for (auto const& test : fillersWithSameName)
            ambiguousTests += test.string() + "\n";
        ETH_ERROR_MESSAGE("Found multiple fillers with the same name. Ambiguous: \n" + ambiguousTests);
    }
}

void checkThatNoTestsWithoutFiller(std::vector<fs::path> const& _verifiedGeneratedTests, fs::path const& _filledTestsPath)
{
    vector<fs::path> compiledTests = test::getFiles(_filledTestsPath, {".json"});
    for (auto const& verifiedTest : _verifiedGeneratedTests)
    {
        auto removed = std::remove_if(compiledTests.begin(), compiledTests.end(),
            [&verifiedTest](fs::path const& x) { return (x.stem() == verifiedTest.stem());} );
        compiledTests.erase(removed, compiledTests.end());
    }
    if (compiledTests.size() > 0)
    {
        string message = "Compiled test folder contains tests without Filler: ";
        for (auto const& test : compiledTests)
            message += "\n " + test.string();
        message += "\n";
        ETH_ERROR_MESSAGE(message);
    }
}

void removePythonTechnicalFiles(std::vector<fs::path>& _allTestFillers)
{
    auto removed = std::remove_if(_allTestFillers.begin(), _allTestFillers.end(),
        [](fs::path const& x) { return (x.stem() == "__init__");} );
    _allTestFillers.erase(removed, _allTestFillers.end());
}

void getFillers(
    TestSuite::AbsoluteFillerPath const& _fullPathToFillers,
    string const& _testNameFilter,
    std::vector<fs::path>& _allTestFillers)
{
    _allTestFillers = test::getFiles(_fullPathToFillers.path(), {".json", ".yml", ".py"}, _testNameFilter);
    if (!_testNameFilter.empty())
    {
        auto fillerSuffixSelect = test::getFiles(_fullPathToFillers.path(), {".json", ".yml"}, _testNameFilter + "Filler");
        for (auto const& el : fillerSuffixSelect)
            _allTestFillers.emplace_back(el);

        auto fillerSuffixSelectC = test::getFiles(_fullPathToFillers.path(), {".json"}, _testNameFilter + "Copier");
        for (auto const& el : fillerSuffixSelectC)
            _allTestFillers.emplace_back(el);

        if (_allTestFillers.size() == 0)
        {
            // If still not find, check inside python files
            bool foundInPythons = false;
            auto pythonFillers = test::getFiles(_fullPathToFillers.path(), {".py"});
            for (auto const& pythonFiller : pythonFillers)
            {
                auto const testNames = getGeneratedTestNames(pythonFiller);
                for (size_t i = 0; i < testNames.size(); i++)
                {
                    if (testNames.at(i) == _testNameFilter)
                    {
                        _allTestFillers.emplace_back(pythonFiller);
                        foundInPythons = true;
                        break;
                    }
                }
                if (foundInPythons)
                    break;
            }
        }
    }
}

void checkGeneratedTestNameCollisions(
    TestSuite::AbsoluteFillerPath const& _fullPathToFillers,
    std::vector<fs::path> const& _allTestFillers,
    string const& _testNameFilter)
{
    // Fill the names map and check double test names
    if (_testNameFilter.empty())
    {
        for (auto const& filler : _allTestFillers)
            getGeneratedTestNames(filler);
    }
    else
    {
        // Check all fillers even if --singletest provided
        std::vector<fs::path> allTestFillers;
        getFillers(_fullPathToFillers, "", allTestFillers);
        for (auto const& filler : allTestFillers)
            getGeneratedTestNames(filler);
    }
    checkDoubleGeneratedTestNames();
}

void TestSuite::checkFillerExistance(string const& _testFolder,
    std::vector<fs::path>& _outdatedTestFillers,
    std::vector<fs::path>& _allTestFillers) const
{
    TestInfo errorInfo("CheckFillers", "");
    TestOutputHelper::get().setCurrentTestInfo(errorInfo);

    string testNameFilter = getTestNameFilter();
    AbsoluteFilledTestPath filledTestsPath = createPathIfNotExist(getFullPathFilled(_testFolder));
    AbsoluteFillerPath fullPathToFillers = getFullPathFiller(_testFolder);

    getFillers(fullPathToFillers, testNameFilter, _allTestFillers);
    removePythonTechnicalFiles(_allTestFillers);
    checkFillersNotWithTheSameName(_allTestFillers, testNameFilter);

    if (!Options::get().forceupdate)
    {
        // Check Hashes and Tests without fillers
        std::vector<fs::path> verifiedGeneratedTests;
        checkGeneratedTest(filledTestsPath.path(),  _allTestFillers, _outdatedTestFillers, verifiedGeneratedTests);

        if (testNameFilter.empty())
            checkThatNoTestsWithoutFiller(verifiedGeneratedTests, filledTestsPath.path());
    }

    checkGeneratedTestNameCollisions(fullPathToFillers, _allTestFillers, testNameFilter);
}


bool TestSuite::verifyFillers(string const& _testFolder,
    std::vector<fs::path>& _outdatedTestFillers,
    std::vector<fs::path>& _allTestFillers) const
{
    // check that destination folder test files has according Filler file in src folder
    try
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("checkFillerExistance", _testFolder));
        checkFillerExistance(_testFolder, _outdatedTestFillers, _allTestFillers);
    }
    catch (std::exception const&)
    {
        TestOutputHelper::get().initTest(1);
        TestOutputHelper::get().finishTest();
        return false;
    }
    return true;
}


}  // namespace test
