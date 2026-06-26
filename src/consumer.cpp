#include "consumer.hpp"
#include "ring_buffer.hpp"
#include "thread_pool.hpp"
#include "signal_processor.hpp"

#include <iostream>
#include <chrono>
#include <mutex>

Consumer::Consumer(
    ThreadSafeQueue<SignalSample> &queue,
    std::mutex &cout_mutex,
    int num_samples,
    int window_size,
    int hop_size,
    double sample_rate,
    int num_workers)
    : queue_(queue),
      cout_mutex_(cout_mutex),
      num_samples_(num_samples),
      window_size_(window_size),
      hop_size_(hop_size),
      sample_rate_(sample_rate),
      num_workers_(num_workers)
{
}

void Consumer::run()
{
    RingBuffer ring_buffer(window_size_);
    ThreadPool pool(num_workers_);

    int window_count = 0;
    int samples_since_last_window = 0;
    bool first_window_done = false;

    long long latency_sum = 0;
    int latency_count = 0;

    for (int i = 0; i < num_samples_; ++i)
    {
        SignalSample sample = queue_.pop();

        auto now = std::chrono::steady_clock::now();

        auto latency =
            std::chrono::duration_cast<std::chrono::microseconds>(
                now - sample.timestamp)
                .count();

        latency_sum += latency;
        latency_count++;

        ring_buffer.push(sample.value);

        if (!ring_buffer.isFull())
        {
            continue;
        }

        if (!first_window_done)
        {
            first_window_done = true;
        }
        else
        {
            ++samples_since_last_window;

            if (samples_since_last_window < hop_size_)
            {
                continue;
            }

            samples_since_last_window = 0;
        }

        ++window_count;

        auto window = ring_buffer.getWindow();

        double sample_rate = sample_rate_;
        int window_size = window_size_;
        int current_window = window_count;

        double avg_queue_latency =
            static_cast<double>(latency_sum) / latency_count;

        latency_sum = 0;
        latency_count = 0;

        pool.submit([window,
                     sample_rate,
                     window_size,
                     current_window,
                     avg_queue_latency,
                     this]()
                    {
            SignalProcessor processor;

            auto start = std::chrono::steady_clock::now();

            double rms = processor.calculateRMS(window);
            auto spectrum = processor.computeFFT(window);

            double peak = processor.findPeakFrequency(
                spectrum,
                sample_rate,
                window_size);

            auto end = std::chrono::steady_clock::now();

            auto process_time =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    end - start).count();

            std::lock_guard<std::mutex> lock(cout_mutex_);

            std::cout
                << "Window " << current_window
                << " | RMS = " << rms
                << " | Peak Freq = " << peak
                << " Hz"
                << " | Avg Queue Latency = "
                << avg_queue_latency
                << " us"
                << " | Process Time = "
                << process_time
                << " us\n";
                });
    }
}