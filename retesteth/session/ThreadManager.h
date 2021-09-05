#pragma once
#include <stdio.h>
#include <vector>
#include <thread>
#include <functional>

class ThreadManager
{
public:
    static ThreadManager& get() {
        static ThreadManager instance;
        return instance;
    }

    void joinThreads(bool _all = true);
    void addTask(std::function<void()> _task);
    bool threadsSleeping() const;

private:
    ThreadManager() {}
    bool _threadsSleeping();

    size_t getMaxAllowedThreads();
    std::vector<std::thread> threadVector;
    unsigned int currConfigId = 0;
    size_t m_maxAllowedThreads = 0;
    bool m_threadsSleeping = false;
};
