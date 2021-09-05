#include "Session.h"
#include "ThreadManager.h"
#include <retesteth/Options.h>
#include <retesteth/ExitHandler.h>
#include <mutex>
#include <future>

mutex g_threadVectorAccess;
mutex g_threadSleepingAccess;
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
    m_maxAllowedThreads = maxAllowedThreads;
    return maxAllowedThreads;
}

void ThreadManager::addTask(std::function<void()> _task)
{
    std::promise<void> thread_starter;
    {
        std::lock_guard<std::mutex> lock(g_threadVectorAccess);

        /*RPCSession::sessionPrint();
        size_t i = 0;
        for (auto const& el : threadVector)
            std::cerr << i++ << " " << el.get_id() << "   ";
        std::cerr << std::endl;*/

        // See how many connections we can afford on current running configuration
        static size_t maxAllowedThreads = getMaxAllowedThreads();
        ClientConfig const& currConfig = Options::get().getDynamicOptions().getCurrentConfig();
        if (currConfigId != currConfig.getId().id())
        {
            currConfigId = currConfig.getId().id();
            maxAllowedThreads = getMaxAllowedThreads();
        }

        // Wait for at least one connection to finish it's task
        if (threadVector.size() == maxAllowedThreads)
            joinThreads(false);

        thread job([_task, starter_future = thread_starter.get_future()]() {
            starter_future.wait();
            _task();
        });
        threadVector.push_back(std::move(job));
        std::lock_guard<std::mutex> lock2(g_threadSleepingAccess);
        m_threadsSleeping = _threadsSleeping();
    }
    thread_starter.set_value();
}

bool ThreadManager::_threadsSleeping()
{
    if (threadVector.size() < m_maxAllowedThreads)
        return true;
    return false;
    /*size_t number = 0;
    for (auto const& el : threadVector)
    {
        auto status = RPCSession::sessionStatus(el.get_id());
        if (status == RPCSession::HasFinished || RPCSession::Available)
            number++;
    }
    return number > 2;*/
}

bool ThreadManager::threadsSleeping() const
{
    std::lock_guard<std::mutex> lock(g_threadSleepingAccess);
    return m_threadsSleeping;
}

void ThreadManager::joinThreads(bool _all)
{
    if (_all)
    {
        bool working = true;
        while (working)
        {
            working = false;
            {
                std::lock_guard<std::mutex> lock(g_threadVectorAccess);
                for (vector<thread>::iterator it = threadVector.begin(); it != threadVector.end(); it++)
                {
                    auto status = RPCSession::sessionStatus((*it).get_id());
                    if (status == RPCSession::HasFinished)
                    {
                        thread::id const id = (*it).get_id();
                        (*it).join();
                        RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
                        threadVector.erase(it);
                        working = true;
                        break;
                    }
                    else if (status == RPCSession::Working)
                        working = true;
                    else if (status == RPCSession::NotExist && ExitHandler::receivedExitSignal())
                        return;
                }
            }
            // The session availability check is time consuming. Let other threads to work
            if (Options::get().threadCount > 1)
                std::this_thread::sleep_for(50ms);
        }

        /*
        for (auto& th : threadVector)
        {
            thread::id const id = th.get_id();
            th.join();
            // A thread with exception thrown still being joined here!
            RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
        }
        threadVector.clear();
        if (ExitHandler::receivedExitSignal())
        {
            // if one of the tests threads failed with fatal exception
            // stop retesteth execution
            ExitHandler::doExit();
        }
        return;  // otherwise continue test execution*/
    }

    bool finished = false;
    while (!finished)
    {
        if (threadVector.size() == 0)
            return;
        for (vector<thread>::iterator it = threadVector.begin(); it != threadVector.end(); it++)
        {
            auto status = RPCSession::sessionStatus((*it).get_id());
            if (status == RPCSession::HasFinished)
            {
                thread::id const id = (*it).get_id();
                (*it).join();
                RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
                threadVector.erase(it);
                return;
            }
            else if (status == RPCSession::NotExist && ExitHandler::receivedExitSignal())
                return;
        }

        // The session availability check is time consuming. Let other threads to work
        if (Options::get().threadCount > 1)
            std::this_thread::sleep_for(50ms);
    }
}
