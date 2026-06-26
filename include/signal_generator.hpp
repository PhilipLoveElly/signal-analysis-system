#ifndef SIGNAL_GENERATOR_HPP
#define SIGNAL_GENERATOR_HPP

#include <vector>

class SignalGenerator {
public:
    SignalGenerator(double sample_rate, double frequency, double amplitude);

    std::vector<double> generateSineWave(int num_samples);

private:
    double sample_rate_;
    double frequency_;
    double amplitude_;
};

#endif
