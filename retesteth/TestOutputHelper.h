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
#include <boost/filesystem.hpp>
#include <libdevcore/CommonData.h>
#include <vector>

namespace test
{

struct TestInfo
{
    TestInfo(std::string const&  _fork, size_t _trD, size_t _trG, size_t _trV)
        : m_sFork(_fork), m_trD(_trD), m_trG(_trG), m_trV(_trV)
    {
        m_isStateTransactionInfo = true;
    }

    TestInfo(std::string const&  _fork, size_t _block)
        : m_sFork(_fork), m_blockNumber(_block), m_isStateTransactionInfo(false)
    {
        m_isBlockchainTestInfo = true;
    }

    TestInfo(): m_isStateTransactionInfo(false), m_isBlockchainTestInfo(false) {}
    std::string getMessage() const;

private:
    std::string m_sFork;
    size_t m_trD, m_trG, m_trV;
    size_t m_blockNumber;
    bool m_isStateTransactionInfo = false;
    bool m_isBlockchainTestInfo = false;
};

class TestOutputHelper
{
public:
    static TestOutputHelper& get();
    //TestOutputHelper(TestOutputHelper const&) = delete;
    void operator=(TestOutputHelper const&) = delete;

    void initTest(size_t _maxTests = 1);
    // Display percantage of completed tests to std::out. Has to be called before execution of every
    // test.
    void showProgress();
    void finishTest();
    static void finisAllTestsManually();

    //void setMaxTests(int _count) { m_maxTests = _count; }
    bool checkTest(std::string const& _testName);
    void markError(std::string const& _message)
    {
        m_errors.push_back(_message + m_testInfo.getMessage());
    }
    std::vector<std::string> const& getErrors() const { return m_errors;}
    void resetErrors() { m_errors.clear(); }
    void setCurrentTestFile(boost::filesystem::path const& _name) { m_currentTestFileName = _name; }
    void setCurrentTestName(std::string const& _name) { m_currentTestName = _name; }
    void setCurrentTestInfo(TestInfo const& _info) { m_testInfo = _info; }
    TestInfo const& testInfo() const { return m_testInfo; }
    std::string const& testName() const { return m_currentTestName; }
    std::string const& caseName() const { return m_currentTestCaseName; }
    boost::filesystem::path const& testFile() const { return m_currentTestFileName; }
    static void printTestExecStats();
    static bool isAllTestsFinished();

    /// get string representation of current threadID
    static std::string getThreadID();

    // Mark the _folderName as executed for a given _suitePath (to filler files)
    static void markTestFolderAsFinished(
        boost::filesystem::path const& _suitePath, std::string const& _folderName);

private:
    TestOutputHelper() {}
    void printBoostError();

private:
    dev::Timer m_timer;
    size_t m_currTest;
    size_t m_maxTests;
    std::string m_currentTestName;
    std::string m_currentTestCaseName;
    TestInfo m_testInfo;
    bool m_isRunning;
    boost::filesystem::path m_currentTestFileName;
    std::vector<std::string> m_errors; //flag errors for triggering boost erros after all thread finished
};

class TestOutputHelperFixture
{
public:
    TestOutputHelperFixture() { TestOutputHelper::get().initTest(); }
    ~TestOutputHelperFixture() { TestOutputHelper::get().finishTest(); }
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
