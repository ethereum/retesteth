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

#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/session/RPCImpl.h>
#include <retesteth/session/ToolImpl.h>
#include <csignal>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
namespace fs = boost::filesystem;

namespace test::session
{
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
        totalRuns = 0;
    }
    std::unique_ptr<RPCSession> session;
    std::unique_ptr<FILE> filePipe;
    int pipePid;
    RPCSession::SessionStatus isUsed;
    std::string tmpDir;
    test::ClientConfigID configId;
    size_t totalRuns;
};

void closeSession(thread::id const& _threadID);

std::mutex g_socketMapMutex;
static std::map<thread::id, sessionInfo> socketMap;

void RPCSession::runNewInstanceOfAClient(thread::id const& _threadID, ClientConfig const& _config)
{
    switch (_config.cfgFile().socketType())
    {
    case ClientConfgSocketType::IPC:
    {
        fs::path tmpDir = test::createUniqueTmpDirectory();
        string ipcPath = tmpDir.string() + "/geth.ipc";

        string command = "bash";
        std::vector<string> args;
        args.emplace_back(_config.getShellPath().c_str());
        args.emplace_back(tmpDir.string());
        args.emplace_back(ipcPath);

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
            ClientConfig const& curCFG = Options::getDynamicOptions().getCurrentConfig();
            size_t const initTime = curCFG.cfgFile().initializeTime();
            std::this_thread::sleep_for(std::chrono::seconds(initTime));
        }
        sessionInfo info(
            fp, new RPCSession(new RPCImpl(Socket::SocketType::IPC, ipcPath)), tmpDir.string(), pid, _config.getId());
        socketMap.insert(std::pair<thread::id, sessionInfo>(_threadID, std::move(info)));
        break;
    }
    case ClientConfgSocketType::TCP:
    {
        Options const& opt = Options::get();
        std::vector<IPADDRESS> const& ports =
            (opt.nodesoverride.size() > 0 ? opt.nodesoverride : _config.cfgFile().socketAdresses());

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
                ETH_DC_MESSAGE(DC::RPC, "addr: " + addr.asString());
                socketMap.insert(std::pair<thread::id, sessionInfo>(_threadID, std::move(info)));
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
        socketMap.insert(std::pair<thread::id, sessionInfo>(_threadID, std::move(info)));
        break;
    }

    case ClientConfgSocketType::TransitionTool:
    {
        fs::path tmpDir = test::createUniqueTmpDirectory();
        sessionInfo info(NULL, new RPCSession(new ToolImpl(Socket::SocketType::TCP, _config.cfgFile().shell(), tmpDir)),
            tmpDir.string(), 0, _config.getId());
        socketMap.insert(std::pair<thread::id, sessionInfo>(_threadID, std::move(info)));
        break;
    }
    default:
        ETH_FAIL_MESSAGE("Unknown Socket Type in runNewInstanceOfAClient");
    }
}

void RPCSession::currentCfgCountTestRun()
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    ClientConfig const& curCFG = Options::getDynamicOptions().getCurrentConfig();
    for (auto& el : socketMap)
    {
        sessionInfo& info = el.second;
        if (info.configId.id() == curCFG.getId().id())
            info.totalRuns++;
    }
}

bool RPCSession::isRunningTooLong()
{
    static const size_t c_maxTestBeforeFlush = 1500;
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    ClientConfig const& curCFG = Options::getDynamicOptions().getCurrentConfig();
    for (auto const& el : socketMap)
    {
        sessionInfo const& info = el.second;
         if (info.configId.id() == curCFG.getId().id() && info.totalRuns > c_maxTestBeforeFlush)
             return true;
    }
    return false;
}

void RPCSession::restartScripts(bool _stop)
{
    ClientConfig const& curCFG = Options::getDynamicOptions().getCurrentConfig();

    if (curCFG.cfgFile().socketType() == ClientConfgSocketType::TCP)
    {
        // if --nodes are set to connect, ignore restart scripts
        if (Options::get().nodesoverride.size() > 0)
            return;
    }

    if (_stop)
    {
        auto stop = [&curCFG](){
            if (fs::exists(curCFG.getStopperScript().c_str()))
            {
                // Close all active connection listeners
                ETH_DC_MESSAGE(DC::RPC, "Restart Client Scripts...");
                RPCSession::clear();
            }
        };
        switch (curCFG.cfgFile().socketType())
        {
        case ClientConfgSocketType::IPC: stop(); return;
        case ClientConfgSocketType::TCP: stop(); return;
        default: break;
        }
        return;
    }

    // If there are no clients started with this configuration, run the start script
    // Assume here that socketMap is open for single configuration at a time only
    if (socketMap.empty())
    {
        if (!fs::exists(curCFG.getStartScript()))
            return;

        size_t const threads = Options::get().threadCount;
        string const start = curCFG.getStartScript().c_str();
        auto cmd = [](string const& _cmd, string const& _args) {
            int exitCode;
            test::executeCmd(_cmd + " " + _args, exitCode, ExecCMDWarning::NoWarning);
        };
        switch (curCFG.cfgFile().socketType())
        {
        case ClientConfgSocketType::TCP:
        {
            thread task(cmd, start, test::fto_string(threads) + " 2>/dev/null");
            ETH_DC_MESSAGE(DC::RPC, start);
            task.detach();
            size_t const initTime = curCFG.cfgFile().initializeTime();
            size_t const seconds = Options::get().lowcpu ? initTime * 5 : initTime;
            this_thread::sleep_for(chrono::seconds(seconds));
        }
        break;
        default:
            break;
        }
    }
}

SessionInterface& RPCSession::instance(thread::id const& _threadID)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    bool needToCreateNew = false;
    test::ClientConfigID currentConfigId = Options::getDynamicOptions().getCurrentConfig().getId();
    if (socketMap.count(_threadID) && socketMap.at(_threadID).configId != currentConfigId)
    {
        // For this thread a session is opened but it is opened not for current tested client
        ETH_FAIL_MESSAGE("A session opened for another client id!");
    }

    // If there are no clients running, instantiate them with starter scripts
    restartScripts();

    if (!socketMap.count(_threadID))
    {
        // look for free clients that already instantiated
        for (auto& socket : socketMap)
        {
            if (socket.second.isUsed == SessionStatus::Available)
                if (socket.second.configId == currentConfigId)
                {
                    socket.second.isUsed = SessionStatus::Working;
                    socketMap.insert(std::pair<thread::id, sessionInfo>(_threadID, std::move(socket.second)));
                    socketMap.erase(socketMap.find(socket.first));  // remove previous threadID assigment to this socket
                    assert(socketMap.count(_threadID));
                    return socketMap.at(_threadID).session.get()->getImplementation();
                }
        }
        needToCreateNew = true;
    }
    if (needToCreateNew)
    {
        size_t const threadID = std::hash<std::thread::id>()(_threadID);
        ETH_DC_MESSAGE(DC::SOCKET, "Run new connection session for `" + test::fto_string(threadID) + "`");
        runNewInstanceOfAClient(_threadID, Options::getDynamicOptions().getCurrentConfig());
        ETH_DC_MESSAGE(DC::SOCKET, "New instance started");
        if (!Options::get().checkhash && socketMap.count(_threadID))
        {
            auto& impl = socketMap.at(_threadID).session.get()->getImplementation();
            ETH_DC_MESSAGE(DC::STATS, "Instantiated: " + impl.web3_clientVersion()->asJson(0, false));
        }
    }

    ETH_FAIL_REQUIRE_MESSAGE(socketMap.size() <= Options::get().threadCount,
        "Something went wrong. Retesteth connect to more instances than needed!");
    ETH_FAIL_REQUIRE_MESSAGE(socketMap.size() != 0, "Something went wrong. Retesteth failed to create socket connection!");
    size_t const threadID = std::hash<std::thread::id>()(_threadID);
    ETH_FAIL_REQUIRE_MESSAGE(
        socketMap.count(_threadID), "ThreadID: `" + fto_string(threadID) + "` not registered in socketMap!");
    return socketMap.at(_threadID).session.get()->getImplementation();
}

void RPCSession::sessionStart(thread::id const& _threadID)
{
    RPCSession::instance(_threadID);  // initialize the client if not exist
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        socketMap.at(_threadID).isUsed = SessionStatus::Working;
}

void RPCSession::sessionEnd(thread::id const& _threadID, SessionStatus _status)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        socketMap.at(_threadID).isUsed = _status;
}

RPCSession::SessionStatus RPCSession::sessionStatus(thread::id const& _threadID)
{
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    if (socketMap.count(_threadID))
        return socketMap.at(_threadID).isUsed;
    return RPCSession::NotExist;
}

void closeSession(thread::id const& _threadID)
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
    // Close all active connection listeners
    std::lock_guard<std::mutex> lock(g_socketMapMutex);
    std::vector<thread> closingThreads;
    for (auto& element : socketMap)
        closingThreads.emplace_back(thread(closeSession, element.first));

    for (auto& th : closingThreads)
        th.join();

    socketMap.clear();
    closingThreads.clear();

    // If not running UnitTests or smth
    auto const& dynOpt = Options::getDynamicOptions();
    if (dynOpt.activeConfigs() > 0 && dynOpt.currentConfigIsSet())
    {
        ClientConfig const& curCFG = dynOpt.getCurrentConfig();
        if (!curCFG.getStopperScript().empty() && Options::get().nodesoverride.size() == 0)
        {
            int exitCode;
            executeCmd(curCFG.getStopperScript().c_str(), exitCode, ExecCMDWarning::NoWarningNoError);
            ETH_DC_MESSAGE(DC::RPC, curCFG.getStopperScript().c_str());
            if (!ExitHandler::receivedExitSignal() && curCFG.cfgFile().socketType() != ClientConfgSocketType::TransitionTool)
            {
                size_t const initTime = curCFG.cfgFile().initializeTime();
                size_t const seconds = Options::get().lowcpu ? initTime + 10 : initTime;
                this_thread::sleep_for(chrono::seconds(seconds));
            }
        }
    }
}

RPCSession::RPCSession(SessionInterface* _impl) : m_implementation(_impl) {}

}  // namespace test::session
