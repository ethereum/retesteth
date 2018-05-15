#include <retesteth/ExitHandler.h>
#include <mutex>
#include <thread>

std::mutex g_executionMutex;
bool ExitHandler::m_shouldExit = false;
bool ExitHandler::m_finishedExecution = false;

void ExitHandler::exitHandler(int)
{
	m_shouldExit = true;
	int waitTime = 10;
	while (!m_finishedExecution && waitTime--)  //m_finishedExecution should have a copy on each thread!!!
	{
		/* wait for all tests to finish */
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	RPCSession::clear();
}

void ExitHandler::setFinishExecution(bool _status)
{
	std::lock_guard<std::mutex> lock(g_executionMutex);
	m_finishedExecution = _status;
}


