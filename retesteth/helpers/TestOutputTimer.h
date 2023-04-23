#pragma once
#include <libdevcore/Common.h>

namespace test {

class TestOutputTimer
{
public:
    TestOutputTimer();
    void restart();
    void printFinishTest(std::string const&) const;
    static void printTotalTimes();
private:
    double getTotalTimer() const { return m_timerTotal.elapsed(); }
    double getTotalCPU() const { return m_timerCPU.elapsed(); }
private:
    dev::Timer m_timerTotal;
    dev::Timer m_timerClient;
    dev::CPUTimer m_timerCPU;
};

}
