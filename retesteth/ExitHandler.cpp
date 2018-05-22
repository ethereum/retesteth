#include <retesteth/ExitHandler.h>
#include <mutex>
#include <thread>

std::mutex g_executionMutex;
bool ExitHandler::m_shouldExit = false;

bool ExitHandler::shouldExit()
{
    std::lock_guard<std::mutex> lock(g_executionMutex);
    return m_shouldExit;
}

void ExitHandler::couldExit()
{
    RPCSession::clear();
}

void ExitHandler::exitHandler(int)
{
    std::lock_guard<std::mutex> lock(g_executionMutex);
    m_shouldExit = true;
}


