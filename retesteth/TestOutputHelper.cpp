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

using namespace std;
using namespace dev;
using namespace test;
using namespace boost;


typedef std::pair<double, std::string> execTimeName;
static std::vector<execTimeName> execTimeResults;

void TestOutputHelper::initTest(size_t _maxTests)
{
	m_currentTestName = "n/a";
    m_currentTestFileName = string();
	m_timer = Timer();
	m_timer.restart();
	m_currentTestCaseName = boost::unit_test::framework::current_test_case().p_name;
	if (!Options::get().createRandomTest)
		std::cout << "Test Case \"" + m_currentTestCaseName + "\": \n";
	m_maxTests = _maxTests;
	m_currTest = 0;
}

bool TestOutputHelper::checkTest(std::string const& _testName)
{
	if (test::Options::get().singleTest && test::Options::get().singleTestName != _testName)
		return false;

	//cnote << _testName;
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
	if (Options::get().exectimelog)
	{
		execTimeName res;
		res.first = m_timer.elapsed();
		res.second = caseName();
		std::cout << res.second + " time: " + toString(res.first) << "\n";
        std::lock_guard<std::mutex> lock(g_resultsUpdate_mutex);
        execTimeResults.push_back(res);
	}
}

void TestOutputHelper::printTestExecStats()
{
    std::lock_guard<std::mutex> lock(g_resultsUpdate_mutex);
	if (Options::get().exectimelog)
	{
        int totalTime = 0;
		std::cout << std::left;
        std::sort(execTimeResults.begin(), execTimeResults.end(), [](execTimeName _a, execTimeName _b) { return (_b.first < _a.first); });
        for (size_t i = 0; i < execTimeResults.size(); i++)
            totalTime += execTimeResults[i].first;
        std::cout << setw(45) << "Total Time: " << setw(25) << "     : " + toString(totalTime) << "\n";
        for (size_t i = 0; i < execTimeResults.size(); i++)
            std::cout << setw(45) << execTimeResults[i].second << setw(25) << " time: " + toString(execTimeResults[i].first) << "\n";
	}
    execTimeResults.clear();
}

std::string TestOutputHelper::getThreadID()
{
    return toString(std::this_thread::get_id());
}

