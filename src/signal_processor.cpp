#include "signal_processor.hpp"
#include "signal_sample.hpp"

#include <cmath>
#include <complex>
#include <vector>

double SignalProcessor::findPeakFrequency(
    const std::vector<std::complex<double>> &spectrum,
    double sample_rate,
    int window_size)
{
    int max_index = 0;
    double max_magnitude = 0.0;

    for (size_t k = 1; k < spectrum.size() / 2; ++k)
    {
        double magnitude = std::abs(spectrum[k]);

        if (magnitude > max_magnitude)
        {
            max_magnitude = magnitude;
            max_index = k;
        }
    }

    return static_cast<double>(max_index) * sample_rate / window_size;
}
double SignalProcessor::calculateRMS(
    const std::vector<double> &signal)
{
    double sum = 0.0;

    for (double value : signal)
    {
        sum += value * value;
    }

    return std::sqrt(sum / signal.size());
}

std::vector<std::complex<double>>
SignalProcessor::computeFFT(const std::vector<double> &signal)
{
    std::vector<std::complex<double>> x;

    for (double value : signal)
    {
        x.emplace_back(value, 0.0);
    }

    fft(x);

    return x;
}
void SignalProcessor::fft(std::vector<std::complex<double>>& x)
{
    int N = x.size();

    // bit-reversal reorder
    for (int i = 1, j = 0; i < N; ++i)
    {
        int bit = N >> 1;

        for (; j & bit; bit >>= 1)
        {
            j ^= bit;
        }

        j ^= bit;

        if (i < j)
        {
            std::swap(x[i], x[j]);
        }
    }

    const double pi = 3.14159265358979323846;

    // butterfly
    for (int len = 2; len <= N; len <<= 1)
    {
        double angle = -2.0 * pi / len;

        std::complex<double> wlen(
            std::cos(angle),
            std::sin(angle));

        for (int i = 0; i < N; i += len)
        {
            std::complex<double> w(1.0, 0.0);

            for (int j = 0; j < len / 2; ++j)
            {
                std::complex<double> u = x[i + j];
                std::complex<double> v = x[i + j + len / 2] * w;

                x[i + j] = u + v;
                x[i + j + len / 2] = u - v;

                w *= wlen;
            }
        }
    }
}