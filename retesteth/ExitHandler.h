#pragma once
#include <retesteth/RPCSession.h>

class ExitHandler
{
public:
	static void exitHandler(int);
    static bool shouldExit();
    static void couldExit();

private:
    static bool m_shouldExit;
};


