#pragma once

#include <mutex>
#include <vector>

class RingBuffer
{
public:
    RingBuffer(size_t size) : buffer(size), max(size) {}

    void push(double v)
    {
        std::lock_guard<std::mutex> guard(mtx);
        buffer[head] = v;
        head = (head + 1) % max;
        if (count < max)
            count++;
    }

    void copy(std::vector<double> &out)
    {
        std::lock_guard<std::mutex> guard(mtx);
        out.resize(count);
        for (size_t i = 0; i < count; i++)
        {
            size_t idx = (head + max - count + i) % max;
            out[i] = buffer[idx];
        }
    }

    // Problem: buffer.data() returns a POINTER to the vector not a copy of it. 
    // double* toArray() {
    //     return buffer.data();
    // }

    size_t size()
    {
        std::lock_guard<std::mutex> guard(mtx);
        return count;
    }

    // chronological indexing
    double chronoIndex(const int i) {
        return buffer[(head + max - count + i) % max];
    }

private:
    std::mutex mtx;
    std::vector<double> buffer;
    size_t head = 0;
    size_t count = 0;
    size_t max;
};
