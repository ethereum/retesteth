#pragma once
#include <retesteth/RPCSession.h>

class ExitHandler
{
public:
	static void exitHandler(int);
	static bool shouldExit() { return m_shouldExit; }
	static void setFinishExecution(bool _status);

private:
	static bool m_shouldExit;
	static bool m_finishedExecution;
};


