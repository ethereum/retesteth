#pragma once
#include <libdevcore/Common.h>

namespace test {

class TestOutputTimer
{
public:
    TestOutputTimer();
    void restart();
    void startSubcallTimer();
    void finishSubcallTimer();
    void printFinishTest(std::string const&) const;
    static void printTotalTimes();
    static void resetT8NTime();
private:
    double getTotalTimer() const { return m_timerTotal.elapsed(); }
    double getTotalCPU() const { return m_timerCPU.elapsed(); }
private:
    dev::Timer m_timerTotal;
    dev::Timer m_timerClient;
    dev::CPUTimer m_timerCPU;
};

}
