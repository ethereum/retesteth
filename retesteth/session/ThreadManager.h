#pragma once
#include <stdio.h>
#include <map>
#include <thread>
#include <functional>

namespace test::session
{
// Manages jobs ensuring that only as many as -j flag allows are currently running
// Construct over the Session class which manages new connections to the clients
class ThreadManager
{
public:
    static void joinThreads(bool _all = true);
    static void addTask(std::function<void()> _job);
private:
    ThreadManager() {}
    static void waitForAtLeastOneJobToFinish();
    static size_t getMaxAllowedThreads();
    static std::map<std::thread::id, std::thread> threadMap;
    static unsigned int currConfigId;
};

}  // namespace test::session
