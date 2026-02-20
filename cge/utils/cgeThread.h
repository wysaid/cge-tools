/*
 * cgeThread.h
 *
 *  Created on: 2015-3-17
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef _CGE_THREAD_H_
#define _CGE_THREAD_H_

// A simple thread model wrapper based on C++11 thread library

#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <memory>
#include <functional>

namespace CGE
{
template <class Type> class CGESyncReadWriteQueue
{
public:
    void putData4Write(const Type& data)
    {
        m_writeMutex.lock();
        m_list4Write.push_back(data);
        m_writeMutex.unlock();
    }

    void putData4Read(const Type& data)
    {
        m_readMutex.lock();
        m_list4Read.push_back(data);
        m_readMutex.unlock();
    }

    bool hasData4Write()
    {
        std::lock_guard<std::mutex> lock(m_writeMutex);
        bool canWrite = !m_list4Write.empty();
        return canWrite;
    }

    bool hasData4Read()
    {
        std::lock_guard<std::mutex> lock(m_readMutex);
        bool canRead = !m_list4Read.empty();
        return canRead;
    }

    Type getData4Write()
    {
        std::unique_lock<std::mutex> lock(m_writeMutex);
        if (m_list4Write.empty()) return Type();

        Type data = m_list4Write.front();
        m_list4Write.pop_front();
        return data;
    }

    Type getData4Read()
    {
        std::unique_lock<std::mutex> lock(m_readMutex);
        if (m_list4Read.empty()) return Type();

        Type data = m_list4Read.front();
        m_list4Read.pop_front();
        return data;
    }

    std::list<Type>& list4Read() { return m_list4Read; }

    std::list<Type>& list4Write() { return m_list4Write; }

private:
    std::list<Type> m_list4Read;
    std::list<Type> m_list4Write;
    std::mutex m_readMutex;
    std::mutex m_writeMutex;
};

/*
class:     CGEThreadPreemptive

Use case: real-time screen refresh, live slider result updates, etc.

Description: Tasks are long-running and persistent, performing similar work continuously
           When a new task arrives, the running task can be discarded immediately or the queued task replaced. At most
one task will be waiting. A single CGEThreadPreemptive instance creates at most one thread.

Note: Avoid calling the API of the same CGEThreadPreemptive from multiple threads simultaneously
*/

class CGEThreadPreemptive
{
public:
    CGEThreadPreemptive();
    ~CGEThreadPreemptive();

    inline bool isActive() const { return m_runningStatus; }
    inline bool isRunning() const { return m_thread != nullptr; }

    void run();
    // void terminate(); // Not yet available
    void quit();

    void join();  // Once started, this thread will not terminate automatically unless quit() is called or the object is
                  // destroyed

protected:
    virtual void runTask() = 0;  // Must be implemented by subclass

    void _run();

public:
    std::mutex& getMutex() { return m_mutex; }
    std::thread* getThread() { return m_thread; }
    std::condition_variable& getCondition() { return m_condition; }

protected:
    std::thread* m_thread;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    bool m_taskRestart, m_threadOver;
    bool m_runningStatus;
};

//////////////////////////////////////////////////////////////////////////

class CGEThreadPool
{
private:
    class Worker;

public:
    class Work;

    CGEThreadPool(std::list<std::unique_ptr<Worker>>::size_type maxThreadNum = 1);
    ~CGEThreadPool();

    bool isActive();  // Returns true if at least one thread is active
    bool isBusy();    // Returns true if all threads are busy

    // Wait while at least one thread is still working; ms=0 means wait until no threads are active
    void wait4Active(long ms = 0);
    // Wait while the thread pool is busy; ms=0 means wait until not busy
    void wait4Busy(long ms = 0);

    void terminate();
    void quit();  // Clear all pending work, wait for all threads to finish, then release all resources and return.

    void join();  // Release all resources and return after all work is complete

    std::list<Work>::size_type totalWorks() { return m_workList.size(); }
    std::list<std::unique_ptr<Worker>>::size_type totalWorkers() { return m_workerList.size(); }

    void run(const Work& work);

private:
    std::list<Work> m_workList;
    std::list<std::unique_ptr<Worker>> m_workerList;

    std::condition_variable m_condition;
    std::mutex m_threadMutex, m_poolMutex;
    std::list<std::unique_ptr<Worker>>::size_type m_maxWorkerSize;
    bool m_threadOver, m_threadJoining;
};

class CGEThreadPool::Work
{
public:
    Work() : func(nullptr), arg(NULL) {}
    Work(const std::function<void(void*)>& _func, void* _arg = NULL) : func(_func), arg(_arg) {}

    void run()
    {
        if (func != nullptr) func(arg);
    }

private:
    std::function<void(void*)> func;
    void* arg;
};

class CGEThreadPool::Worker
{
public:
    Worker(CGEThreadPool& pool);
    Worker(Worker& worker);
    Worker(Worker&& worker);
    ~Worker();

    inline bool isActive() const { return m_runningStatus; }
    inline bool isRunning() const { return m_thread != nullptr; }

    void shouldLeave() { m_shouldLeave = true; }

    void run();
    void terminate();
    void waitForQuit();

    void join();

protected:
    void _run();

private:
    std::thread* m_thread;
    CGEThreadPool& m_pool;
    bool m_runningStatus, m_shouldLeave;
};
}  // namespace CGE

#define CGE_THREAD_WORK CGE::CGEThreadPool::Work

#endif