#pragma once

#include <boost/noncopyable.hpp>
#include <boost/test/unit_test.hpp>

#include <retesteth/ethObjects/common.h>
#include <retesteth/session/SessionInterface.h>
#include <stdio.h>
#include <map>
#include <string>

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

    static SessionInterface& instance(std::string const& _threadID);
    static void sessionStart(std::string const& _threadID);
    static void sessionEnd(std::string const& _threadID, SessionStatus _status);
    static SessionStatus sessionStatus(std::string const& _threadID);
    static void clear();

    SessionInterface& getImplementation() { return *m_implementation; }
    ~RPCSession() { delete m_implementation; }

private:
    explicit RPCSession(SessionInterface* _impl);
    static void runNewInstanceOfAClient(std::string const& _threadID, ClientConfig const& _config);
    SessionInterface* m_implementation;

    /// Parse std::string replacing keywords to values
    // void parseString(std::string& _string, std::map<std::string, std::string> const& _varMap);
};
