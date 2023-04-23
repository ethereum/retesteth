#include "TestOutputTimer.h"
#include <retesteth/helpers/TestHelper.h>
using namespace std;

namespace  {
    typedef std::pair<double, std::string> cpuTimeName;
    typedef std::pair<double, cpuTimeName> execTimeName;

    std::mutex g_execTimeResults;
    static std::vector<execTimeName> execTimeResults;
}

namespace test {

TestOutputTimer::TestOutputTimer()
{
    restart();
}

void TestOutputTimer::restart()
{
    m_timerTotal = dev::Timer();
    m_timerCPU = dev::CPUTimer();
}

void TestOutputTimer::printFinishTest(string const& _testName) const
{
    execTimeName res;
    res.first = getTotalTimer();
    res.second = {getTotalCPU(), _testName};
    auto const& time = res.first;
    auto const& cputime = res.second.first;
    std::cout << setprecision(3)
              << res.second.second + " time: " << time
              << ", cputime: " << cputime
              << " (" << (int)floor(100 * cputime / std::max(1., time)) << "%)"
              << "\n";
    std::lock_guard<std::mutex> lock(g_execTimeResults);
    execTimeResults.emplace_back(res);
}

void TestOutputTimer::printTotalTimes()
{
    std::lock_guard<std::mutex> lock(g_execTimeResults);
    double totalTime = 0;
    double totalTimeCPU = 0;
    std::cout << std::left;
    std::sort(execTimeResults.begin(), execTimeResults.end(), [](execTimeName _a, execTimeName _b) { return (_b.first < _a.first); });
    for (size_t i = 0; i < execTimeResults.size(); i++)
    {
        totalTime += execTimeResults[i].first;
        totalTimeCPU += execTimeResults[i].second.first;
    }
    std::cout << "*** Execution time stats" << std::endl;
    std::cout << setprecision(3)
              << setw(40) << "Total Time: "
              << "     : "   << setw(5) << totalTime
              << "        : "<< setw(5) << totalTimeCPU
              << "(" << (int)floor(100 * totalTimeCPU / std::max(1., totalTime)) << "%)"
              << "\n";
    for (size_t i = 0; i < execTimeResults.size(); i++)
    {
        auto const& totalTime = execTimeResults[i].first;
        auto const& cputime = execTimeResults[i].second.first;
        std::cout << setprecision(3)
                  << setw(40) << execTimeResults[i].second.second
                  << " time: "    << setw(5) << totalTime
                  << " cputime: " << setw(5) << cputime
                  << "(" << (int)floor(100 * cputime / std::max(1., totalTime)) << "%)"
                  << "\n";
    }
    std::cout << "\n";
    execTimeResults.clear();
}

}
