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

class TestOutputHelper
{
public:
    static TestOutputHelper& get();
    //TestOutputHelper(TestOutputHelper const&) = delete;
	void operator=(TestOutputHelper const&) = delete;

	void initTest(size_t _maxTests = 1);
	// Display percantage of completed tests to std::out. Has to be called before execution of every test.
	void showProgress();
	void finishTest();

	//void setMaxTests(int _count) { m_maxTests = _count; }
	bool checkTest(std::string const& _testName);
    void markError(std::string const& _message)
    {
        m_errors.push_back(_message + " (" + m_currentTestName + ")");
    }
    std::vector<std::string> const& getErrors() const { return m_errors;}
    void setCurrentTestFile(boost::filesystem::path const& _name) { m_currentTestFileName = _name; }
	void setCurrentTestName(std::string const& _name) { m_currentTestName = _name; }
	std::string const& testName() { return m_currentTestName; }
	std::string const& caseName() { return m_currentTestCaseName; }
	boost::filesystem::path const& testFile() { return m_currentTestFileName; }
        static void printTestExecStats();

        /// get string representation of current threadID
        static std::string getThreadID();

      private:
	TestOutputHelper() {}
	dev::Timer m_timer;
	size_t m_currTest;
	size_t m_maxTests;
	std::string m_currentTestName;
	std::string m_currentTestCaseName;
    boost::filesystem::path m_currentTestFileName;
    std::vector<std::string> m_errors; //flag errors for triggering boost erros after all thread finished
    void printBoostError();
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
