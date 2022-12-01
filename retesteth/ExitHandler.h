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


