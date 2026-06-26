#include "producer.hpp"

#include <iostream>
#include <chrono>
#include <thread>

Producer::Producer(ThreadSafeQueue<SignalSample> &queue,
                   std::mutex &cout_mutex,
                   double sample_rate,
                   double frequency,
                   double amplitude,
                   int num_samples,
                   int window_size)
    : queue_(queue),
      cout_mutex_(cout_mutex),
      generator_(sample_rate, frequency, amplitude),
      sample_rate_(sample_rate),
      frequency_(frequency),
      amplitude_(amplitude),
      num_samples_(num_samples),
      window_size_(window_size)
{
}

void Producer::run()
{
    auto signal =
        generator_.generateSineWave(num_samples_);

   /* auto next_time =
        std::chrono::steady_clock::now();

    auto sample_period =
        std::chrono::duration<double>(
            1.0 / sample_rate_);*/

    int count = 0;

    for (double value : signal)
    {
        SignalSample sample;
        sample.value = value;
        sample.timestamp =
            std::chrono::steady_clock::now();

        queue_.push(sample);

        ++count;

       /*next_time +=
            std::chrono::duration_cast<
                std::chrono::steady_clock::duration>(
                sample_period);

        std::this_thread::sleep_until(next_time);*/
    }

    std::lock_guard<std::mutex> lock(cout_mutex_);

    std::cout << "Produced "
              << count
              << " samples\n";
}