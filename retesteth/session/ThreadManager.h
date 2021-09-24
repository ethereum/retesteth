#pragma once
#include <stdio.h>
#include <map>
#include <thread>
#include <functional>

class ThreadManager
{
public:
    static void joinThreads(bool _all = true);
    static void addTask(std::function<void()> _job);
private:
    ThreadManager() {}
    static size_t getMaxAllowedThreads();
    static std::map<std::thread::id, std::thread> threadMap;
    static unsigned int currConfigId;
};
