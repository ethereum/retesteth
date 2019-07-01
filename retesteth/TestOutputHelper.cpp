/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file
 * Fixture class for boost output when running testeth
 */

#include <mutex>
#include <thread>
#include <boost/test/unit_test.hpp>
#include <libdevcore/include.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/Options.h>
#include <retesteth/ExitHandler.h>
#include <libdevcore/Log.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace boost;

mutex g_finishedTestFoldersMapMutex;
typedef std::set<std::string> FolderNameSet;
static std::map<boost::filesystem::path, FolderNameSet> finishedTestFoldersMap;
// static std::map<boost::filesystem::path, FolderNameSet> exceptionTestFoldersMap;
void checkUnfinishedTestFolders();  // Checkup that all test folders are active during the test run

typedef std::pair<double, std::string> execTimeName;
static std::vector<execTimeName> execTimeResults;
static int execTotalErrors = 0;
static std::map<std::string, TestOutputHelper> helperThreadMap; // threadID => outputHelper
static int numberOfRunningTests = 0;
mutex g_numberOfRunningTests;

mutex g_helperThreadMapMutex;
TestOutputHelper& TestOutputHelper::get()
{
    std::lock_guard<std::mutex> lock(g_helperThreadMapMutex);
    if (helperThreadMap.count(getThreadID()))
        return helperThreadMap.at(getThreadID());
    else
    {
        TestOutputHelper instance;
        helperThreadMap.emplace(std::make_pair(getThreadID(), std::move(instance)));
        helperThreadMap.at(getThreadID()).initTest(0);
    }
    return helperThreadMap.at(getThreadID());
}

void TestOutputHelper::finisAllTestsManually()
{
    // thread safe?
    for (auto& helper : helperThreadMap)
        helper.second.finishTest();
}

void TestOutputHelper::initTest(size_t _maxTests)
{
    //_maxTests = 0 means this function is called from testing thread
    m_currentTestName = string();
    m_currentTestInfo = string();
    m_currentTestFileName = string();
    m_timer = Timer();
    m_currentTestCaseName = boost::unit_test::framework::current_test_case().p_name;
    m_isRunning = false;
    if (!Options::get().createRandomTest && _maxTests != 0)
    {
        std::cout << "Test Case \"" + m_currentTestCaseName + "\": \n";
        std::lock_guard<std::mutex> lock(g_numberOfRunningTests);
        numberOfRunningTests++;
        m_timer.restart();
        m_isRunning = true;
    }
    m_maxTests = _maxTests;
    m_currTest = 0;
}

bool TestOutputHelper::checkTest(std::string const& _testName)
{
	if (test::Options::get().singleTest && test::Options::get().singleTestName != _testName)
		return false;

    m_currentTestName = _testName;
	return true;
}

void TestOutputHelper::showProgress()
{
	m_currTest++;
	int m_testsPerProgs = std::max(1, (int)(m_maxTests / 4));
	if (!test::Options::get().createRandomTest && (m_currTest % m_testsPerProgs == 0 || m_currTest ==  m_maxTests))
	{
		int percent = int(m_currTest*100/m_maxTests);
		std::cout << percent << "%";
		if (percent != 100)
			std::cout << "...";
		std::cout << "\n";
	}
}

std::mutex g_resultsUpdate_mutex;
void TestOutputHelper::finishTest()
{
    if (!m_isRunning)
        return;
    m_isRunning = false;
    if (Options::get().exectimelog)
	{
		execTimeName res;
		res.first = m_timer.elapsed();
		res.second = caseName();
		std::cout << res.second + " time: " + toString(res.first) << "\n";
        std::lock_guard<std::mutex> lock(g_resultsUpdate_mutex);
        execTimeResults.push_back(res);
	}
    printBoostError();  // !! could delete instance of TestOutputHelper !!
    std::lock_guard<std::mutex> lock(g_numberOfRunningTests);
    numberOfRunningTests--;
}

void TestOutputHelper::printBoostError()
{
    size_t errorCount = 0;
    std::lock_guard<std::mutex> lock(g_helperThreadMapMutex);
    for (auto& test : helperThreadMap)
    {
        errorCount += test.second.getErrors().size();
        for (auto const& a : test.second.getErrors())
            ETH_STDERROR_MESSAGE("Error: " + a);
        test.second.resetErrors();
    }
    if (errorCount)
    {
        ETH_STDERROR_MESSAGE("\n--------");
        ETH_STDERROR_MESSAGE(
            "TestOutputHelper detected " + toString(errorCount) + " errors during test execution!");
        std::lock_guard<std::mutex> lock(g_resultsUpdate_mutex);
        BOOST_ERROR("");  // NOT THREAD SAFE !!!
        execTotalErrors += errorCount;
    }
    // helperThreadMap.clear(); !!! could not delete TestHelper from TestHelper destructor !!!
}

bool TestOutputHelper::isAllTestsFinished()
{
    std::lock_guard<std::mutex> lock(g_numberOfRunningTests);
    return numberOfRunningTests <= 0;
}

void TestOutputHelper::printTestExecStats()
{
    checkUnfinishedTestFolders();
    std::lock_guard<std::mutex> lock(g_resultsUpdate_mutex);
    if (Options::get().exectimelog)
    {
        int totalTime = 0;
        std::cout << std::left;
        std::sort(execTimeResults.begin(), execTimeResults.end(), [](execTimeName _a, execTimeName _b) { return (_b.first < _a.first); });
        for (size_t i = 0; i < execTimeResults.size(); i++)
            totalTime += execTimeResults[i].first;
        std::cout << std::endl << "*** Execution time stats" << std::endl;
        std::cout << setw(45) << "Total Time: " << setw(25) << "     : " + toString(totalTime) << "\n";
        for (size_t i = 0; i < execTimeResults.size(); i++)
            std::cout << setw(45) << execTimeResults[i].second << setw(25) << " time: " + toString(execTimeResults[i].first) << "\n";
    }

    if (execTotalErrors)
    {
        ETH_STDERROR_MESSAGE("\n--------");
        ETH_STDERROR_MESSAGE("*** TOTAL ERRORS DETECTED: " + toString(execTotalErrors) +
                             " errors during all test execution!");
        ETH_STDERROR_MESSAGE("--------");
    }
    execTimeResults.clear();
    execTotalErrors = 0;
}

std::string TestOutputHelper::getThreadID()
{
    return toString(std::this_thread::get_id());
}

// check if a boost path contain no test files
bool pathHasTests(boost::filesystem::path const& _path)
{
    using fsIterator = boost::filesystem::directory_iterator;
    for (fsIterator it(_path); it != fsIterator(); ++it)
    {
        // if the extention of a test file
        if (boost::filesystem::is_regular_file(it->path()) &&
            (it->path().extension() == ".json" || it->path().extension() == ".yml"))
        {
            // if the filename ends with Filler/Copier type
            std::string const name = it->path().stem().filename().string();
            std::string const suffix =
                (name.length() > 7) ? name.substr(name.length() - 6) : string();
            if (suffix == "Filler" || suffix == "Copier")
                return true;
        }
    }
    return false;
}

string detectFilterForMinusTArgument()
{
    // -t SuiteName/caseName   parse caseName as filter
    // rCurrentTestSuite is empty if run without -t argument
    Options const& opt = Options::get();
    if (opt.rCurrentTestSuite.empty())
        return string();

    string filter;
    // -t SuiteName/subSuiteName/caseName
    // if (SuiteName/subSuiteName == -t argument) then filter is empty
    bool thereisPathEqualToSuite = false;
    for (auto const& allTestsIt : finishedTestFoldersMap)
    {
        // Remove Filler from folder name so it equal to -t argument
        string parent = allTestsIt.first.parent_path().stem().string();
        size_t index = parent.find("Filler");
        if (index == std::string::npos)
            break;
        parent = parent.replace(index, 6, "");

        string path = parent + "/" + allTestsIt.first.filename().string();
        if (opt.rCurrentTestSuite == path)
        {
            thereisPathEqualToSuite = true;
            break;
        }
    }

    if (!thereisPathEqualToSuite)
    {
        size_t pos = opt.rCurrentTestSuite.find_last_of('/');
        if (pos != string::npos)
            filter = opt.rCurrentTestSuite.substr(pos + 1);
    }

    return filter;
}

void checkUnfinishedTestFolders()
{
    std::lock_guard<std::mutex> lock(g_finishedTestFoldersMapMutex);
    string filter = detectFilterForMinusTArgument();

    if (!filter.empty())
    {
        if (finishedTestFoldersMap.size() > 1)
        {
            ETH_STDERROR_MESSAGE("ERROR: Expected a single test to be passed: " + filter + "\n");
            return;
        }

        // Unit tests does not mark test folders
        if (finishedTestFoldersMap.size() == 0)
            return;

        std::map<boost::filesystem::path, FolderNameSet>::const_iterator it =
            finishedTestFoldersMap.begin();
        if (!pathHasTests(it->first / filter))
            ETH_STDERROR_MESSAGE(string("WARNING: Test folder ") + (it->first / filter).c_str() +
                                 " appears to have no tests!");
    }
    else
    {
        for (auto const& allTestsIt : finishedTestFoldersMap)
        {
            boost::filesystem::path path = allTestsIt.first;
            set<string> allFolders;
            using fsIterator = boost::filesystem::directory_iterator;
            for (fsIterator it(path); it != fsIterator(); ++it)
            {
                if (boost::filesystem::is_directory(*it))
                {
                    string const folderName = it->path().filename().string();
                    allFolders.insert(folderName);
                    if (!pathHasTests(it->path()))
                        ETH_STDERROR_MESSAGE(string("WARNING: Test folder ") + it->path().c_str() +
                                             " appears to have no tests!");
                }
            }

            std::vector<string> diff;
            FolderNameSet finishedFolders = allTestsIt.second;
            std::set_difference(allFolders.begin(), allFolders.end(), finishedFolders.begin(),
                finishedFolders.end(), std::back_inserter(diff));
            for (auto const& it : diff)
                ETH_STDERROR_MESSAGE(string("WARNING: Test folder ") + (path / it).c_str() +
                                     " appears to be unused!");
        }
    }
}


// Mark test folder _folderName as not to be checked for the test suite path _suitePath
/*void TestOutputHelper::markTestFolderAsException(
    boost::filesystem::path const& _suitePath, string const& _folderName)
{
    std::lock_guard<std::mutex> lock(g_finishedTestFoldersMapMutex);
    exceptionTestFoldersMap[_suitePath].emplace(_folderName);
}*/

// Mark test folder _folderName as finished for the test suite path _suitePath
void TestOutputHelper::markTestFolderAsFinished(
    boost::filesystem::path const& _suitePath, string const& _folderName)
{
    std::lock_guard<std::mutex> lock(g_finishedTestFoldersMapMutex);
    finishedTestFoldersMap[_suitePath].emplace(_folderName);
}
