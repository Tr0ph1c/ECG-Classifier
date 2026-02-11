#pragma once

#include <mutex>
#include <vector>

class RingBuffer
{
public:
    RingBuffer(size_t size) : buffer(size), max(size) {}

    void push(float v)
    {
        std::lock_guard<std::mutex> guard(mtx);
        buffer[head] = v;
        head = (head + 1) % max;
        if (count < max)
            count++;
    }

    void copy(std::vector<float> &out)
    {
        std::lock_guard<std::mutex> guard(mtx);
        out.resize(count);
        for (size_t i = 0; i < count; i++)
        {
            size_t idx = (head + max - count + i) % max;
            out[i] = buffer[idx];
        }
    }
    size_t size()
    {
        std::lock_guard<std::mutex> guard(mtx);
        return count;
    }

private:
    std::mutex mtx;
    std::vector<float> buffer;
    size_t head = 0;
    size_t count = 0;
    size_t max;
};
