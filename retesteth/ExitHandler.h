#pragma once
class ExitHandler
{
public:
	static void exitHandler(int);
    static bool receivedExitSignal();
    static void doExit();

private:
    static bool m_receivedExitSig;
};

#define CHECKEXIT                          \
    if (ExitHandler::receivedExitSignal()) \
        return;
#define CHECKEXITR(RET)                    \
    if (ExitHandler::receivedExitSignal()) \
        return RET;
