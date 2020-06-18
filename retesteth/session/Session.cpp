/*
    This file is part of solidity.

    solidity is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    solidity is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with solidity.  If not, see <http://www.gnu.org/licenses/>.

    The Implementation originally from
   https://msdn.microsoft.com/en-us/library/windows/desktop/aa365592(v=vs.85).aspx
*/
/// @file RPCSession.cpp
/// Low-level IPC communication between the test framework and the Ethereum node.

#include "Session.h"

#include <thread>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/configs/ClientConfig.h>
#include <retesteth/session/RPCImpl.h>
#include <retesteth/session/ToolImpl.h>

using namespace std;
using namespace dev;
using namespace test;

struct sessionInfo
{
    sessionInfo(FILE* _pipe, RPCSession* _session, std::string const& _tmpDir, int _pid, ClientConfigID const& _configId)
    {
        session.reset(_session);
        filePipe.reset(_pipe);
        tmpDir = _tmpDir;
        pipePid = _pid;
        isUsed = RPCSession::NotExist;
        configId = _configId;
    }
    std::unique_ptr<RPCSession> session;
    std::unique_ptr<FILE> filePipe;
    int pipePid;
    RPCSession::SessionStatus isUsed;
    std::string tmpDir;
    test::ClientConfigID configId;
};

void closeSession(const string& _threadID);

std::mutex g_socketMapMutex;
static std::map<std::string, sessionInfo> socketMap;  // ! make inside a class static
void RPCSession::runNewInstanceOfAClient(string const& _threadID, ClientConfig const& _config)
{
    switch (_config.cfgFile().socketType())
    {
    case ClientConfgSocketType::IPC:
    {
        fs::path tmpDir = test::createUniqueTmpDirectory();
        string ipcPath = tmpDir.string() + "/geth.ipc";

        string command = "bash";
        std::vector<string> args;
        args.push_back(_config.getShellPath().c_str());
        args.push_back(tmpDir.string());
        args.push_back(ipcPath);

        int pid = 0;
        test::popenOutput mode =
            (Options::get().enableClientsOutput) ? test::popenOutput::EnableALL : test::popenOutput::DisableAll;
        FILE* fp = test::popen2(command, args, "r", pid, mode);
        if (!fp)
        {
            ETH_ERROR_MESSAGE("Failed to start the client: '" + command + "'");
            std::raise(SIGABRT);
        }
        else
        {
            int maxSeconds = 25;
            while (!boost::filesystem::exists(ipcPath) && maxSeconds-- > 0)
                std::this_thread::sleep_for(std::chrono::seconds(1));
            ETH_FAIL_REQUIRE_MESSAGE(maxSeconds > 0, "Client took too long to start ipc!");
            // Client has opened ipc socket. wait for it to initialize
            std::this_thread::sleep_for(std::chrono::seconds(4));
        }
        sessionInfo info(
            fp, new RPCSession(new RPCImpl(Socket::SocketType::IPC, ipcPath)), tmpDir.string(), pid, _config.getId());
        socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
        break;
    }
    case ClientConfgSocketType::TCP:
    {
        std::vector<IPADDRESS> const& ports =
            (Options::get().nodesoverride.size() > 0 ? Options::get().nodesoverride : _config.cfgFile().socketAdresses());

        // Create sessionInfo for a tcp address that is still not present in socketMap
        for (auto const& addr : ports)
        {
            bool unused = true;
            for (auto const& socket : socketMap)
            {
                sessionInfo const& sInfo = socket.second;
                if (sInfo.session.get()->getImplementation().getSocketPath() == addr.asString())
                {
                    unused = false;
                    break;
                }
            }
            if (unused)
            {
                sessionInfo info(
                    NULL, new RPCSession(new RPCImpl(Socket::SocketType::TCP, addr.asString())), "", 0, _config.getId());
                socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
                return;
            }
        }
        break;
    }
    case ClientConfgSocketType::IPCDebug:
    {
        // connect to already opend .ipc socket
        fs::path tmpDir = test::createUniqueTmpDirectory();
        fs::path const& ipcPath = _config.cfgFile().path();
        int pid = 0;
        FILE* fp = NULL;
        sessionInfo info(
            fp, new RPCSession(new RPCImpl(Socket::SocketType::IPC, ipcPath.string())), tmpDir.string(), pid, _config.getId());
        socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
        break;
    }

    case ClientConfgSocketType::TransitionTool:
    {
        sessionInfo info(
            NULL, new RPCSession(new ToolImpl(Socket::SocketType::TCP, _config.cfgFile().shell())), "", 0, _config.getId());
        socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(info)));
        break;
    }
    default:
        ETH_FAIL_MESSAGE("Unknown Socket Type in runNewInstanceOfAClient");
    }
}

SessionInterface& RPCSession::instance(const string _threadID)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    bool needToCreateNew = false;
    test::ClientConfigID currentConfigId = Options::getDynamicOptions().getCurrentConfig().getId();
    if (socketMap.count(_threadID) && socketMap.at(_threadID).configId != currentConfigId)
    {
        // For this thread a session is opened but it is opened not for current tested client
        ETH_FAIL_MESSAGE("A session opened for another client id!");
    }

    if (!socketMap.count(_threadID))
    {
        // look for free clients that already instantiated
        for (auto& socket : socketMap)
        {
            if (socket.second.isUsed == SessionStatus::Available)
                if (socket.second.configId == currentConfigId)
                {
                    socket.second.isUsed = SessionStatus::Working;
                    socketMap.insert(std::pair<string, sessionInfo>(_threadID, std::move(socket.second)));
                    socketMap.erase(socketMap.find(socket.first));  // remove previous threadID assigment to this socket
                    assert(socketMap.count(_threadID));
                    return socketMap.at(_threadID).session.get()->getImplementation();
                }
        }
        needToCreateNew = true;
    }
    if (needToCreateNew)
        runNewInstanceOfAClient(_threadID, Options::getDynamicOptions().getCurrentConfig());

    ETH_FAIL_REQUIRE_MESSAGE(socketMap.size() <= Options::get().threadCount,
        "Something went wrong. Retesteth connect to more instances than needed!");
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.size() != 0, "Something went wrong. Retesteth failed to create socket connection!");
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.count(_threadID), "ThreadID: `" + _threadID + "` not registered in socketMap!");
    return socketMap.at(_threadID).session.get()->getImplementation();
}

void RPCSession::sessionStart(std::string const _threadID)
{
    RPCSession::instance(_threadID);  // initialize the client if not exist
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        socketMap.at(_threadID).isUsed = SessionStatus::Working;
}

void RPCSession::sessionEnd(std::string const _threadID, SessionStatus _status)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    assert(socketMap.count(_threadID));
    if (socketMap.count(_threadID))
        socketMap.at(_threadID).isUsed = _status;
}

RPCSession::SessionStatus RPCSession::sessionStatus(std::string const _threadID)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        return socketMap.at(_threadID).isUsed;
    return RPCSession::NotExist;
}

void closeSession(const string& _threadID)
{
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.count(_threadID), "Socket map is empty in closeSession!");
    sessionInfo& element = socketMap.at(_threadID);
    if (element.session.get()->getImplementation().getSocketType() == Socket::SocketType::IPC)
    {
        test::pclose2(element.filePipe.get(), element.pipePid);
        std::this_thread::sleep_for(std::chrono::seconds(4));
        boost::filesystem::remove_all(boost::filesystem::path(element.tmpDir));
        element.filePipe.release();
        element.session.release();
    }
}

void RPCSession::clear()
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    std::vector<thread> closingThreads;
    for (auto& element : socketMap)
        closingThreads.push_back(thread(closeSession, element.first));
    for (auto& th : closingThreads)
        th.join();

    socketMap.clear();
    closingThreads.clear();
}

RPCSession::RPCSession(SessionInterface* _impl) : m_implementation(_impl) {}
