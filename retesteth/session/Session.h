#pragma once
#include <boost/noncopyable.hpp>
#include <thread>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/session/SessionInterface.h>

namespace test::session
{
// Session connections to an instance of a client
class RPCSession : public boost::noncopyable
{
public:
    enum SessionStatus
    {
        Working,      // test execution in progress
        Available,    // test execution has finished
        HasFinished,  // has just finished execution
        NotExist      // socket yet not initialized
    };

    static SessionInterface& instance(std::thread::id const& _threadID);
    static void sessionStart(std::thread::id const& _threadID);
    static void sessionEnd(std::thread::id const& _threadID, SessionStatus _status);
    static SessionStatus sessionStatus(std::thread::id const& _threadID);
    static void clear();

    // Flush the memory by restarting the clients with configuration scripts
    static void currentCfgCountTestRun();            // Increase test run counter
    static bool isRunningTooLong();                  // True if running connection for tool long
    static void restartScripts(bool _stop = false);  // Stop all connections (flush)

    SessionInterface& getImplementation() { return *m_implementation; }
    ~RPCSession() { delete m_implementation; }

private:
    explicit RPCSession(SessionInterface* _impl);
    static void runNewInstanceOfAClient(std::thread::id const& _threadID, test::ClientConfig const& _config);
    SessionInterface* m_implementation;
};

}  // namespace test::session
