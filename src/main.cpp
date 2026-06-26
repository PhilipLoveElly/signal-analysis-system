#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <complex>
#include <string>

#include "signal_generator.hpp"
#include "thread_safe_queue.hpp"
#include "signal_processor.hpp"
#include "signal_sample.hpp"
#include "producer.hpp"
#include "consumer.hpp"
#include "thread_pool.hpp"

void runSingleThread(double sample_rate,
                     double frequency,
                     double amplitude,
                     int num_samples,
                     int window_size)
{
    SignalGenerator generator(sample_rate, frequency, amplitude);
    SignalProcessor processor;

    auto signal = generator.generateSineWave(num_samples);

    std::vector<double> buffer;
    buffer.reserve(window_size);

    auto total_start =
        std::chrono::steady_clock::now();

    int window_count = 0;

    for (double value : signal)
    {
        buffer.push_back(value);

        if (buffer.size() == static_cast<size_t>(window_size))
        {
            ++window_count;

            auto process_start =
                std::chrono::steady_clock::now();

            double rms = processor.calculateRMS(buffer);
            auto spectrum =
                processor.computeFFT(buffer);

            double peak_freq =
                processor.findPeakFrequency(
                    spectrum,
                    sample_rate,
                    window_size);
            auto process_end =
                std::chrono::steady_clock::now();

            auto process_time =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    process_end - process_start)
                    .count();

            std::cout
                << "[Single] Window " << window_count
                << " | RMS = " << rms
                << " | Peak Freq = " << peak_freq << " Hz"
                << " | Process Time = " << process_time << " us\n";

            buffer.clear();
        }
    }

    auto total_end =
        std::chrono::steady_clock::now();

    auto total_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            total_end - total_start)
            .count();

    std::cout
        << "[Single] Total Runtime = "
        << total_time
        << " us\n";
}
constexpr double SAMPLE_RATE = 100000.0;
constexpr double FREQUENCY = 10000.0;
constexpr double AMPLITUDE = 1.0;
constexpr int NUM_SAMPLES = 8192;
constexpr int WINDOW_SIZE = 1024;
constexpr int HOP_SIZE = 64;

int main(int argc, char *argv[])
{
    int num_workers = 4;

    if (argc > 1)
    {
        num_workers = std::stoi(argv[1]);
    }

    std::cout << "Thread Pool Workers = "
              << num_workers
              << "\n";

    runSingleThread(
        SAMPLE_RATE,
        FREQUENCY,
        AMPLITUDE,
        NUM_SAMPLES,
        WINDOW_SIZE);
    ThreadSafeQueue<SignalSample> queue;
    std::mutex cout_mutex;
    Producer producer(
        queue,
        cout_mutex,
        SAMPLE_RATE,
        FREQUENCY,
        AMPLITUDE,
        NUM_SAMPLES,
        WINDOW_SIZE);
    struct Result
    {
        int window;
        double rms;
        double peak_freq;
        double avg_latency;
        long long process_time_us;
    };
    Consumer consumer(
        queue,
        cout_mutex,
        NUM_SAMPLES,
        WINDOW_SIZE,
        HOP_SIZE,
        SAMPLE_RATE,
        num_workers);

    auto total_start =
        std::chrono::steady_clock::now();

    std::thread producer_thread(
        &Producer::run,
        &producer);

    std::thread consumer_thread(
        &Consumer::run,
        &consumer);

    producer_thread.join();

    consumer_thread.join();

    auto total_end =
        std::chrono::steady_clock::now();

    auto total_time =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            total_end - total_start)
            .count();

    std::cout
        << "Total Runtime = "
        << total_time
        << " ms\n";

    return 0;
}