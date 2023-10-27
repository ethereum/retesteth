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

#pragma once
#include <libdevcore/CommonData.h>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>
#include <retesteth/helpers/TestInfo.h>
#include <retesteth/helpers/TestOutputTimer.h>
#include <thread>
#include <vector>

namespace test
{

class TestOutputHelper
{
public:
    static TestOutputHelper& get();
    TestOutputHelper(TestOutputHelper const&) = default;
    void operator=(TestOutputHelper const&) = delete;

    void initTest(size_t _maxTests = 1);

    // Display percantage of completed tests to std::out.
    // Has to be called before execution of every test.
    void showProgress();
    void finishTest();
    TestOutputTimer& timer() { return m_timer; }

    bool markError(std::string const& _message);
    void markWarning(std::string const& _message);
    void unmarkLastError();
    const std::string c_exception_any = "ANY EXCEPTION";

    // Do not treat next error as error.
    void setUnitTestExceptions(std::vector<std::string> const& _messages);

    std::vector<std::string> const& getUnitTestExceptions() const
    {
        return m_expected_UnitTestExceptions;
    }

    std::vector<std::string> const& getErrors() const { return m_errors;}
    void resetErrors() { m_errors.clear(); }
    void setPythonTestFlag(bool _flag) { m_pythonTestRunning = _flag; }
    void setCurrentTestFile(boost::filesystem::path const& _name) { m_currentTestFileName = _name; }
    void setCurrentTestName(std::string const& _name) { m_currentTestName = _name; }
    void setCurrentTestInfo(TestInfo const& _info) { m_testInfo = _info; }
    TestInfo const& testInfo() const { return m_testInfo; }
    std::string const& testName() const { return m_currentTestName; }
    boost::filesystem::path const& testFile() const { return m_currentTestFileName; }
    bool getPythonTestFlag() const { return m_pythonTestRunning; }
    static void printTestExecStats();
    static void registerTestRunSuccess();
    static void currentTestRunPP() { m_currentTestRun++; };

    /// get string representation of current threadID
    static std::thread::id getThreadID();

    // Debug
    static void addTestVector(std::string&& _str);
    static void printTestVectors();

    // Mark the _folderName as executed for a given _suitePath (to filler files)
    static void markTestFolderAsFinished(
        boost::filesystem::path const& _suitePath, std::string const& _folderName);

private:
    TestOutputHelper() {}
    void printBoostError();
    bool _unmarkExpectedExceptionForTest(std::string const&);

private:
    bool m_pythonTestRunning = false;
    TestOutputTimer m_timer;
    size_t m_currTest;
    size_t m_maxTests;
    std::string m_currentTestName;
    TestInfo m_testInfo;
    boost::filesystem::path m_currentTestFileName;
    std::vector<std::string> m_errors; //flag errors for triggering boost erros after all thread finished
    std::vector<std::string> m_expected_UnitTestExceptions;  // expect following errors

    // Debug print
    static size_t m_currentTestRun;
};

class TestOutputHelperFixture
{
public:
    TestOutputHelperFixture()
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("unit test"));
        TestOutputHelper::get().initTest();
    }
    ~TestOutputHelperFixture()
    {
        TestOutputHelper::get().finishTest();
        TestOutputHelper::get().registerTestRunSuccess();
    }
};

template <class T>
inline std::string expButGot(T _exp, T _got)
{
    return "Expected: '" + std::to_string(_exp) + "', but Got: '" + std::to_string(_got) + "'";
}

inline std::string expButGot(std::string const& _exp, std::string const& _got)
{
    return "Expected: '" + _exp + "', but Got: '" + _got + "'";
}

} //namespace test
