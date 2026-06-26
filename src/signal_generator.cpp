#include "signal_generator.hpp"
#include <cmath>

SignalGenerator::SignalGenerator(double sample_rate, double frequency, double amplitude)
    : sample_rate_(sample_rate),
      frequency_(frequency),
      amplitude_(amplitude)
{
}

std::vector<double> SignalGenerator::generateSineWave(int num_samples)
{
    std::vector<double> signal;
    signal.reserve(num_samples);

    const double pi = 3.14159265358979323846;

    for (int n = 0; n < num_samples; ++n) {
        double t = static_cast<double>(n) / sample_rate_;
        double value = amplitude_ * std::sin(2.0 * pi * frequency_ * t);
        signal.push_back(value);
    }

    return signal;
}
