#include <retesteth/ExitHandler.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <mutex>
#include <thread>
using namespace test;
using namespace test::session;

std::mutex g_executionMutex;
bool ExitHandler::m_receivedExitSig = false;

bool ExitHandler::receivedExitSignal()
{
    std::lock_guard<std::mutex> lock(g_executionMutex);
    return m_receivedExitSig;
}

std::mutex g_runOnce;
void ExitHandler::doExit()
{
    std::lock_guard<std::mutex> lock(g_runOnce);
    static bool runOnce = false;
    if (!runOnce)
    {
        RPCSession::clear();
        test::TestOutputHelper::printTestExecStats();
        runOnce = true;
    }
}

void ExitHandler::exitHandler(int)
{
    std::lock_guard<std::mutex> lock(g_executionMutex);
    m_receivedExitSig = true;
    std::cerr << "Finishing retesteth run" << std::endl;
}


