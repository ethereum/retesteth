#include "ThreadManager.h"
#include "Session.h"
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <condition_variable>

size_t g_activejobs = 0;
std::mutex g_jobsmutex;
std::condition_variable g_cv;
std::mutex g_callbackmutex;
using namespace std;

namespace test::session
{
unsigned int ThreadManager::currConfigId = 0;
map<thread::id, thread> ThreadManager::threadMap;
using namespace test;

size_t ThreadManager::getMaxAllowedThreads()
{
    // If debugging, already there is an open instance of a client.
    // Only one thread allowed to connect to it;
    size_t maxAllowedThreads = Options::get().threadCount;
    ClientConfig const& currConfig = Options::get().getDynamicOptions().getCurrentConfig();
    currConfigId = currConfig.getId().id();
    ClientConfgSocketType socType = currConfig.cfgFile().socketType();
    if (socType == ClientConfgSocketType::IPCDebug)
        maxAllowedThreads = 1;

    // If connecting to TCP sockets. Max threads are limited with tcp ports provided
    if (socType == ClientConfgSocketType::TCP)
    {
        maxAllowedThreads = min(maxAllowedThreads, currConfig.cfgFile().socketAdresses().size());
        if (maxAllowedThreads != Options::get().threadCount)
            ETH_WARNING(
                "Correct -j option to `" + test::fto_string(maxAllowedThreads) + "` (or provide socket ports in config)!");
    }
    return maxAllowedThreads;
}

void ThreadManager::addTask(std::function<void()> _job)
{
    {
         std::lock_guard<std::mutex> lk(g_jobsmutex);
         g_activejobs++;
    }

    auto wrappedJob = [_job](){
        _job();
        g_cv.notify_one();
        std::lock_guard<std::mutex> lk(g_jobsmutex);
        g_activejobs--;
    };
    thread workThread(wrappedJob);
    threadMap.emplace(workThread.get_id(), std::move(workThread));

    // See how many connections we can afford on current running configuration
    static size_t maxAllowedThreads = getMaxAllowedThreads();
    ClientConfig const& currConfig = Options::get().getDynamicOptions().getCurrentConfig();
    if (currConfigId != currConfig.getId().id())
    {
        currConfigId = currConfig.getId().id();
        maxAllowedThreads = getMaxAllowedThreads();
    }

    // Wait for at least one connection to finish it's task
    if (threadMap.size() == maxAllowedThreads)
    {
        waitForAtLeastOneJobToFinish();
        joinThreads(false);
    }
}

void ThreadManager::joinThreads(bool _all)
{
    if (_all)
    {
        for (auto& th : threadMap)
        {
            th.second.join();
            // A thread with exception thrown still being joined here!
            RPCSession::sessionEnd(th.first, RPCSession::SessionStatus::Available);
        }
        threadMap.clear();
        if (ExitHandler::receivedExitSignal())
        {
            // if one of the tests threads failed with fatal exception stop retesteth execution
            ExitHandler::doExit();
        }
        // otherwise continue test execution
        return;
    }

    bool finished = false;
    while (!finished)
    {
        for (std::map<thread::id, thread>::iterator it = threadMap.begin(); it != threadMap.end(); it++)
        {
            auto status = RPCSession::sessionStatus(it->first);
            if (status == RPCSession::HasFinished)
            {
                thread::id const& id = it->first;
                it->second.join();
                RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
                threadMap.erase(it);
                return;
            }
            else if (status == RPCSession::NotExist && ExitHandler::receivedExitSignal())
                return;
        }

        // The session availability check is time consuming. Let other threads to work
        if (Options::get().threadCount > 1)
            std::this_thread::sleep_for(25ms);
    }
}

void ThreadManager::waitForAtLeastOneJobToFinish()
{
    std::unique_lock<std::mutex> lkj(g_jobsmutex);
    if (g_activejobs != 0)
    {
        lkj.unlock();
        std::unique_lock<std::mutex> lk(g_callbackmutex);
        g_cv.wait(lk);
    }
    else
        lkj.unlock();
}
}  // namespace test::session
