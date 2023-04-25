#include "TestOutputTimer.h"
#include <retesteth/helpers/TestHelper.h>
using namespace std;

namespace  {
    // test, time, cputime, t8ntime
    typedef std::tuple<string, double, double, double> execTimeName;

    std::mutex g_execTimeResults;
    static std::vector<execTimeName> execTimeResults;

    std::mutex g_execT8nTimeResults;
    static double currentT8NSubcallTime = 0;
}

namespace test {

TestOutputTimer::TestOutputTimer()
{
    restart();
}

void TestOutputTimer::resetT8NTime()
{
    std::lock_guard<std::mutex> lock(g_execT8nTimeResults);
    currentT8NSubcallTime = 0;
}

void TestOutputTimer::restart()
{
    m_timerTotal = dev::Timer();
    m_timerCPU = dev::CPUTimer();
}

void TestOutputTimer::startSubcallTimer()
{
    m_timerClient.restart();
}

void TestOutputTimer::finishSubcallTimer()
{
    std::lock_guard<std::mutex> lock(g_execT8nTimeResults);
    currentT8NSubcallTime += m_timerClient.elapsed();
}

void TestOutputTimer::printFinishTest(string const& _testName) const
{
    double t8nTimeRecorded;
    {
        std::lock_guard<std::mutex> lock(g_execT8nTimeResults);
        t8nTimeRecorded = currentT8NSubcallTime;
    }
    const execTimeName res = { _testName, getTotalTimer(), getTotalCPU(), t8nTimeRecorded };
    auto const& test = std::get<0>(res);
    auto const& time = std::get<1>(res);
    auto const& cputime = std::get<2>(res);
    auto const& t8ntime = std::get<3>(res);
    std::cout << std::fixed << setprecision(2)
              << test + " time: " << time
              << ", cputime: " << cputime
              << " (" << (int)floor(100 * cputime / std::max(1., time)) << "%)"
              << ", t8ntime: " << t8ntime
              << " (" << (int)floor(100 * t8ntime / std::max(1., time)) << "%)"
              << "\n";
    std::lock_guard<std::mutex> lock(g_execTimeResults);
    execTimeResults.emplace_back(res);
}

void TestOutputTimer::printTotalTimes()
{
    std::lock_guard<std::mutex> lock(g_execTimeResults);
    double totalTime = 0;
    double totalTimeCPU = 0;
    double totalTimeT8N = 0;
    std::cout << std::left;
    std::sort(execTimeResults.begin(), execTimeResults.end(), [](execTimeName _a, execTimeName _b)
        {
            return std::get<1>(_b) < std::get<1>(_a);
        });
    for (size_t i = 0; i < execTimeResults.size(); i++)
    {
        totalTime += std::get<1>(execTimeResults[i]);
        totalTimeCPU += std::get<2>(execTimeResults[i]);
        totalTimeT8N += std::get<3>(execTimeResults[i]);
    }
    std::cout << "*** Execution time stats" << std::endl;
    std::cout << std::fixed << setprecision(2)
              << setw(35) << "Total Time: "
              << "     : "   << setw(6) << totalTime
              << "        : "<< setw(6) << totalTimeCPU
              << "(" << (int)floor(100 * totalTimeCPU / std::max(1., totalTime)) << "%)"
              << "        : "<< setw(6) << totalTimeT8N
              << "(" << (int)floor(100 * totalTimeT8N / std::max(1., totalTime)) << "%)"
              << "\n";
    for (size_t i = 0; i < execTimeResults.size(); i++)
    {
        auto const& test = std::get<0>(execTimeResults[i]);
        auto const& totalTime = std::get<1>(execTimeResults[i]);
        auto const& cputime = std::get<2>(execTimeResults[i]);
        auto const& t8ntime = std::get<3>(execTimeResults[i]);
        std::cout << std::fixed << setprecision(2)
                  << setw(35) << test
                  << " time: "    << setw(6) << totalTime
                  << " cputime: " << setw(6) << cputime
                  << "(" << (int)floor(100 * cputime / std::max(1., totalTime)) << "%)"
                  << " t8ntime: " << setw(6) << t8ntime
                  << "(" << (int)floor(100 * t8ntime / std::max(1., totalTime)) << "%)"
                  << "\n";
    }
    std::cout << "\n";
    execTimeResults.clear();
}

}
