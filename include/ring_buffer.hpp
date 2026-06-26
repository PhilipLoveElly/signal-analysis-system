#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <vector>
#include <cstddef>
#include <stdexcept>

class RingBuffer
{
public:
    explicit RingBuffer(size_t capacity)
        : buffer_(capacity),
          capacity_(capacity),
          head_(0),
          size_(0)
    {
    }

    void push(double value)
    {
        buffer_[head_] = value;
        head_ = (head_ + 1) % capacity_;

        if (size_ < capacity_)
        {
            ++size_;
        }
    }

    bool isFull() const
    {
        return size_ == capacity_;
    }

    std::vector<double> getWindow() const
    {
        if (!isFull())
        {
            throw std::runtime_error("RingBuffer is not full yet");
        }

        std::vector<double> window;
        window.reserve(capacity_);

        size_t start = head_;

        for (size_t i = 0; i < capacity_; ++i)
        {
            size_t index = (start + i) % capacity_;
            window.push_back(buffer_[index]);
        }

        return window;
    }

private:
    std::vector<double> buffer_;
    size_t capacity_;
    size_t head_;
    size_t size_;
};

#endif