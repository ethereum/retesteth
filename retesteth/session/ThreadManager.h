#pragma once
#include <stdio.h>
#include <vector>
#include <thread>

class ThreadManager
{
public:
    static void joinThreads(bool _all = true);
    static void addTask(std::thread _job);
private:
    ThreadManager() {}
    static size_t getMaxAllowedThreads();
    static std::vector<std::thread> threadVector;
    static unsigned int currConfigId;
};
