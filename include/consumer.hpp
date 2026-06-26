#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include <mutex>
#include <vector>

#include "thread_safe_queue.hpp"
#include "signal_processor.hpp"
#include "signal_sample.hpp"

class Consumer
{
public:
    Consumer(
        ThreadSafeQueue<SignalSample> &queue,
        std::mutex &cout_mutex,
        int num_samples,
        int window_size,
        int hop_size,
        double sample_rate,
        int num_workers);

    void run();

private:
    ThreadSafeQueue<SignalSample> &queue_;

    std::mutex &cout_mutex_;

    SignalProcessor processor_;

    int num_samples_;

    int window_size_;

    int hop_size_;

    double sample_rate_;

    int num_workers_;
};

#endif