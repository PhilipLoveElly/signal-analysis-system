#include "thread_pool.hpp"

ThreadPool::ThreadPool(size_t num_threads)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers_.emplace_back([this]()
        {
            while (true)
            {
                std::function<void()> job;

                {
                    std::unique_lock<std::mutex> lock(mutex_);

                    cv_.wait(lock, [this]()
                    {
                        return stop_ || !jobs_.empty();
                    });

                    if (stop_ && jobs_.empty())
                    {
                        return;
                    }

                    job = jobs_.front();
                    jobs_.pop();
                }

                job();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_ = true;
    }

    cv_.notify_all();

    for (auto &worker : workers_)
    {
        worker.join();
    }
}

void ThreadPool::submit(std::function<void()> job)
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        jobs_.push(job);
    }

    cv_.notify_one();
}