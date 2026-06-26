#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <mutex>
#include <vector>

#include "thread_safe_queue.hpp"
#include "signal_generator.hpp"
#include "signal_sample.hpp"

class Producer
{
public:
    Producer(ThreadSafeQueue<SignalSample> &queue,
             std::mutex &cout_mutex,
             double sample_rate,
             double frequency,
             double amplitude,
             int num_samples,
             int window_size);

    void run();

private:
    ThreadSafeQueue<SignalSample> &queue_;
    std::mutex &cout_mutex_;
    SignalGenerator generator_;
    double sample_rate_;
    double frequency_;
    double amplitude_;
    int num_samples_;
    int window_size_;
};

#endif