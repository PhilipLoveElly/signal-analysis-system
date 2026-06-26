#ifndef SIGNAL_PROCESSOR_HPP
#define SIGNAL_PROCESSOR_HPP

#include <vector>
#include <complex>

class SignalProcessor
{
public:
    double calculateRMS(
        const std::vector<double> &signal);
    double findPeakFrequency(
        const std::vector<std::complex<double>> &spectrum,
        double sample_rate,
        int window_size);
    std::vector<std::complex<double>>
    computeFFT(
        const std::vector<double> &signal);

private:
    void fft(std::vector<std::complex<double>> &x);
};

#endif