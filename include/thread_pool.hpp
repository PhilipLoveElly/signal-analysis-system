#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();

    void submit(std::function<void()> job);

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> jobs_;

    std::mutex mutex_;
    std::condition_variable cv_;

    bool stop_ = false;
};

#endif