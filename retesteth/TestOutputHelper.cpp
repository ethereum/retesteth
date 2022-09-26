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

#include <boost/test/tree/test_case_counter.hpp>
#include <boost/test/tree/traverse.hpp>
#include <boost/test/unit_test.hpp>
#include <mutex>
#include <thread>

#include <libdevcore/include.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestOutputHelper.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace boost;
using namespace boost::unit_test;

mutex g_finishedTestFoldersMapMutex;
typedef std::set<std::string> FolderNameSet;
static std::map<boost::filesystem::path, FolderNameSet> finishedTestFoldersMap;
// static std::map<boost::filesystem::path, FolderNameSet> exceptionTestFoldersMap;
void checkUnfinishedTestFolders();  // Checkup that all test folders are active during the test run

typedef std::pair<double, std::string> execTimeName;
static std::vector<execTimeName> execTimeResults;
static int execTotalErrors = 0;
static std::map<thread::id, TestOutputHelper> helperThreadMap;  // threadID => outputHelper
mutex g_totalTestsRun;
mutex g_failedTestsMap;
mutex g_execTotalErrors;
static int totalTestsRun = 0;
static std::map<std::string, std::string> s_failedTestsMap;

mutex g_helperThreadMapMutex;
TestOutputHelper& TestOutputHelper::get()
{
    std::lock_guard<std::mutex> lock(g_helperThreadMapMutex);
    thread::id const tID = getThreadID();
    if (helperThreadMap.count(tID))
        return helperThreadMap.at(tID);
    else
    {
        TestOutputHelper instance;
        helperThreadMap.emplace(std::make_pair(tID, std::move(instance)));
        helperThreadMap.at(tID).initTest(0);
    }
    return helperThreadMap.at(tID);
}

bool TestOutputHelper::markError(std::string const& _message)
{
    // We might have an expected exceptions to happen. See if it is like that
    if (m_expected_UnitTestExceptions.size() > 0)
    {
        // STRING SIMPLIFICATION
        string rmessage = _message;
        auto cond = [](int el) { return (el == ' ' || el == '\n' || el == '\t'); };
        rmessage.erase(std::remove_if(rmessage.begin(), rmessage.end(), cond), rmessage.end());

        auto eraseAllSubStr = [](std::string& mainStr, const std::string& toErase) {
            size_t pos = std::string::npos;
            while ((pos = mainStr.find(toErase)) != std::string::npos)
                mainStr.erase(pos, toErase.length());
        };
        auto eraseSubStrings = [&](std::string& mainStr, const std::vector<std::string>& strList) {
            std::for_each(strList.begin(), strList.end(), std::bind(eraseAllSubStr, std::ref(mainStr), std::placeholders::_1));
        };
        eraseSubStrings(rmessage, {cRed, cYellow});
        // ------------------------

        string const& allowedException =
            m_expected_UnitTestExceptions.at(m_expected_UnitTestExceptions.size() - 1);

        if (_message.find(allowedException) != string::npos || allowedException == c_exception_any ||
            rmessage.find(allowedException) != string::npos)
        {
            m_expected_UnitTestExceptions.pop_back();
            return false;
        }
        else
        {
            string printMessage = _message;
            if (_message.find(cYellow) != std::string::npos)
                printMessage = rmessage;
            ETH_STDERROR_MESSAGE("Occured error: \n--------\n" + printMessage + "\n--------\n vs Expected error: \n--------\n" +
                                 allowedException + "\n--------\n");
        }
    }

    // Mark the error
    string const testDebugInfo = m_testInfo.errorDebug();
    m_errors.push_back(_message + testDebugInfo);
    if (testDebugInfo.empty())
        ETH_WARNING(TestOutputHelper::get().testName() + ", Message: " + _message +
                    ", has empty debugInfo! Missing debug Testinfo for test step.");
    std::lock_guard<std::mutex> lock(g_failedTestsMap);
    if (!s_failedTestsMap.count(TestOutputHelper::get().testName()))
        s_failedTestsMap[TestOutputHelper::get().testName()] = testDebugInfo;
    return true;
}

// When ToolImpl imitate the client side, it uses retesteth logic. but the error is not a failure on
// retesteth side
void TestOutputHelper::unmarkLastError()
{
    if (m_errors.size())
        m_errors.pop_back();
    std::lock_guard<std::mutex> lock(g_failedTestsMap);
    string const& tname = TestOutputHelper::get().testName();
    if (s_failedTestsMap.count(tname))
        s_failedTestsMap.erase(tname);
}

void TestOutputHelper::setUnitTestExceptions(std::vector<std::string> const& _messages)
{
    m_expected_UnitTestExceptions = _messages;
}

size_t TestOutputHelper::m_currentTestRun = 0;
void TestOutputHelper::initTest(size_t _maxTests)
{
    static size_t totalTestsNumber = 0;
    if (totalTestsNumber == 0)
    {
        // Calculate total number of test suites by traversing boost test suite
        test_case_counter tcc;
        traverse_test_tree(boost::unit_test::framework::master_test_suite(), tcc, true);
        totalTestsNumber = tcc.p_count.get();
        m_currentTestRun = 0;
        if (totalTestsNumber == 0)
            totalTestsNumber = 1;
    }

    //_maxTests = 0 means this function is called from testing thread
    m_currentTestName = string();
    m_currentTestFileName = string();
    m_timer = Timer();
    if (_maxTests != 0 && !Options::get().singleTestFile.initialized())
    {
        string testOutOf = "(" + test::fto_string(++m_currentTestRun) + " of " + test::fto_string(totalTestsNumber) + ")";
        ETH_DC_MESSAGE(DC::STATS, "Test Case \"" + TestInfo::caseName() + "\": " + testOutOf);
        m_timer.restart();
    }
    m_maxTests = _maxTests;
    m_currTest = 0;
    m_expected_UnitTestExceptions.clear();
}

void TestOutputHelper::registerTestRunSuccess()
{
    std::lock_guard<std::mutex> lock(g_totalTestsRun);
    totalTestsRun++;
}

void TestOutputHelper::showProgress()
{
    m_currTest++;
    int m_testsPerProgs = std::max(1, (int)(m_maxTests / 4));
    if (m_currTest % m_testsPerProgs == 0 || m_currTest ==  m_maxTests)
    {
        ETH_FAIL_REQUIRE_MESSAGE(m_maxTests > 0, "TestHelper has 0 or negative m_maxTests!");
        ETH_FAIL_REQUIRE_MESSAGE(
            m_currTest <= m_maxTests, "TestHelper has m_currTest > m_maxTests!");
        assert(m_maxTests > 0);
        int percent = int(m_currTest*100/m_maxTests);
        string const ending = (percent != 100) ? "..." : "";
        ETH_DC_MESSAGE(DC::STATS, test::fto_string(percent) + "%" + ending);
    }
}

std::mutex g_execTimeResults;
void TestOutputHelper::finishTest()
{
    auto const& opt = Options::get();
    if (opt.exectimelog && !opt.singleTestFile.initialized())
    {
        std::cout << "Tests finished: " << m_currTest << std::endl;
        execTimeName res;
        res.first = m_timer.elapsed();
        res.second = TestInfo::caseName();
        std::cout << res.second + " time: " + fto_string(res.first) << "\n";
        std::lock_guard<std::mutex> lock(g_execTimeResults);
        execTimeResults.push_back(res);
    }
    printBoostError();  // !! could delete instance of TestOutputHelper !!
}

void TestOutputHelper::printBoostError()
{
    size_t errorCount = 0;
    std::lock_guard<std::mutex> lock(g_helperThreadMapMutex);
    for (auto& test : helperThreadMap)
    {
        TestOutputHelper& threadLocalHelper = test.second;
        errorCount += threadLocalHelper.getErrors().size();
        for (auto const& err : threadLocalHelper.getErrors())
            ETH_STDERROR_MESSAGE("Error: " + err);
        threadLocalHelper.resetErrors();
    }
    if (errorCount)
    {
        ETH_STDERROR_MESSAGE("\n--------");
        ETH_STDERROR_MESSAGE("TestOutputHelper detected " + fto_string(errorCount) + " errors during test execution!");
        std::lock_guard<std::mutex> lock(g_execTotalErrors);
        BOOST_ERROR("");  // NOT THREAD SAFE !!!
        execTotalErrors += errorCount;
    }
    // helperThreadMap.clear(); !!! could not delete TestHelper from TestHelper destructor !!!
}

void TestOutputHelper::printTestExecStats()
{
    auto const& opt = Options::get();
    if (!(opt.singleTestFile.initialized() || opt.customTestFolder.initialized()))
        checkUnfinishedTestFolders();

    if (Options::get().exectimelog)
    {
        std::lock_guard<std::mutex> lock(g_execTimeResults);
        double totalTime = 0;
        std::cout << std::left;
        std::sort(execTimeResults.begin(), execTimeResults.end(), [](execTimeName _a, execTimeName _b) { return (_b.first < _a.first); });
        for (size_t i = 0; i < execTimeResults.size(); i++)
            totalTime += execTimeResults[i].first;
        std::cout << std::endl << "*** Execution time stats" << std::endl;
        {
            std::lock_guard<std::mutex> lock2(g_totalTestsRun);
            if (totalTestsRun > 0)
                std::cout << setw(45) << "Total Tests: " << setw(25) << "     : " + fto_string(totalTestsRun) << "\n";
            else
                ETH_STDERROR_MESSAGE("*** Total Tests Run: " + fto_string(totalTestsRun) + "\n");
        }
        std::cout << setw(45) << "Total Time: " << setw(25) << "     : " + fto_string(totalTime) << "\n";
        for (size_t i = 0; i < execTimeResults.size(); i++)
            std::cout << setw(45) << execTimeResults[i].second << setw(25) << " time: " + fto_string(execTimeResults[i].first) << "\n";
        std::cout << "\n";
    }
    else
    {
        std::lock_guard<std::mutex> lock(g_totalTestsRun);
        string message = "*** Total Tests Run: " + fto_string(totalTestsRun) + "\n";
        if (totalTestsRun > 0)
            ETH_STDOUT_MESSAGE(message);
        else
            ETH_STDERROR_MESSAGE(message);
    }

    bool wereExecErrors = false;
    {
        std::lock_guard<std::mutex> lock(g_execTotalErrors);
        if (execTotalErrors)
        {
            wereExecErrors = true;
            ETH_STDERROR_MESSAGE("\n--------");
            ETH_STDERROR_MESSAGE("*** TOTAL ERRORS DETECTED: " + toString(execTotalErrors) +
                                 " errors during all test execution!");
            ETH_STDERROR_MESSAGE("--------");
        }
    }

    if (wereExecErrors)
    {
        std::lock_guard<std::mutex> lock(g_failedTestsMap);
        for (auto const& error : s_failedTestsMap)
            std::cout << "info:" << error.second << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(g_execTimeResults);
        execTimeResults.clear();
    }

    {
        std::lock_guard<std::mutex> lock(g_execTotalErrors);
        execTotalErrors = 0;
    }
}

thread::id TestOutputHelper::getThreadID()
{
    return std::this_thread::get_id();
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

void checkUnfinishedTestFolders()
{
    std::lock_guard<std::mutex> lock(g_finishedTestFoldersMapMutex);
    // Unit tests does not mark test folders
    if (finishedTestFoldersMap.size() == 0)
        return;

    if (Options::get().rCurrentTestSuite.empty())
        ETH_WARNING("Options rCurrentTestSuite is empty, total tests run can be wrong!");

    // -t SuiteName/SubSuiteName/caseName   parse caseName as filter
    // rCurrentTestSuite is empty if run without -t argument
    string filter;
    size_t pos = Options::get().rCurrentTestSuite.rfind('/');
    if (pos != string::npos)
        filter = Options::get().rCurrentTestSuite.substr(pos + 1);

    std::map<boost::filesystem::path, FolderNameSet>::const_iterator singleTest =
        finishedTestFoldersMap.begin();
    if (!filter.empty() && boost::filesystem::exists(singleTest->first / filter))
    {
        if (finishedTestFoldersMap.size() > 1)
        {
            ETH_STDERROR_MESSAGE("ERROR: Expected a single test to be passed: " + filter + "\n");
            return;
        }

        if (!pathHasTests(singleTest->first / filter))
            ETH_WARNING(string("Test folder ") + (singleTest->first / filter).c_str() +
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
                        ETH_WARNING(string("Test folder ") + it->path().c_str() +
                                    " appears to have no tests!");
                }
            }

            std::vector<string> diff;
            FolderNameSet finishedFolders = allTestsIt.second;
            std::set_difference(allFolders.begin(), allFolders.end(), finishedFolders.begin(),
                finishedFolders.end(), std::back_inserter(diff));
            for (auto const& it : diff)
                ETH_WARNING(string("Test folder ") + (path / it).c_str() + " appears to be unused!");
        }
    }
}


// Mark test folder _folderName as finished for the test suite path _suitePath
void TestOutputHelper::markTestFolderAsFinished(
    boost::filesystem::path const& _suitePath, string const& _folderName)
{
    std::lock_guard<std::mutex> lock(g_finishedTestFoldersMapMutex);
    finishedTestFoldersMap[_suitePath].emplace(_folderName);
}


TestInfo::TestInfo(std::string const& _info, std::string const& _testName) : m_sFork(_info)
{
    m_isGeneralTestInfo = true;
    m_currentTestCaseName = boost::unit_test::framework::current_test_case().p_name;
    if (!_testName.empty())
        TestOutputHelper::get().setCurrentTestName(_testName);
}

std::string TestInfo::errorDebug() const
{
    if (m_sFork.empty())
        return "";
    string message = cYellow + " (" + m_currentTestCaseName + "/" + TestOutputHelper::get().testName();

    if (!m_isGeneralTestInfo)
    {
        message += ", fork: " + m_sFork;
        if (!m_sChainName.empty())
            message += ", chain: " + m_sChainName;
    }
    else
        message += ", step: " + m_sFork;

    if (m_isBlockchainTestInfo)
        message += ", block: " + to_string(m_blockNumber);
    else if (m_isStateTransactionInfo)
        message += ", TrInfo: d: " + to_string(m_trD) + ", g: " + to_string(m_trG) + ", v: " + to_string(m_trV);

    if (!m_sTransactionData.empty())
        message += ", TrData: `" + m_sTransactionData + "`";

    return message + ")" + cDefault;
}
