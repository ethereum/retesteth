#pragma once
#include <retesteth/session/Session.h>

class ExitHandler
{
public:
	static void exitHandler(int);
    static bool receivedExitSignal();
    static void doExit();

private:
    static bool m_receivedExitSig;
};


