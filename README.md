# Linux C++ Multithreaded Real-Time Signal Analysis System

A Linux-based real-time signal processing framework implemented in C++.

## Features

- Producer-Consumer pipeline
- Thread-safe queue
- Ring buffer overlap processing
- FFT-based frequency analysis
- RMS calculation
- Queue latency profiling
- Thread pool parallel execution
- Throughput benchmarking

## Architecture

```text
Producer
    ↓
ThreadSafeQueue
    ↓
Consumer
    ↓
Ring Buffer
    ↓
Overlap Window
    ↓
Thread Pool
    ↓
FFT + RMS
```

## Benchmark

| Workers | Average Runtime |
|----------|----------------|
| 1 | 20.2 ms |
| 2 | 10.7 ms |
| 4 | 6.5 ms |
| 8 | 6.1 ms |
| 10 | 6.0 ms |
| 16 | 7.2 ms |
| 32 | 8.1 ms |

## Future Work

- Lock-Free Queue
- CPU Affinity
- SIMD / FFTW optimization
- TCP network streaming
- Adaptive pipeline scheduling
