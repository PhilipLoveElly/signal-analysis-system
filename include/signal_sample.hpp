#ifndef SIGNAL_SAMPLE_HPP
#define SIGNAL_SAMPLE_HPP

#include <chrono>

struct SignalSample
{
    double value;

    std::chrono::steady_clock::time_point timestamp;
};

#endif