#pragma once
#include <fstream>
#include <vector>
#include <mutex>
#include <functional>
#include <thread>

class SignalProducer {
private:
    // vector because we need a dynamic array (recording size not known beforehand)
    static std::vector<float> fullRecording;
    static size_t fullRecIndex;
    
    static const int SHAREDBUFFERSIZE = 4096;
    static float buffer[SHAREDBUFFERSIZE];
    static int bufferIndex;

    // Mutex Lock
    static std::mutex lock;

    static bool isRunning;
    static constexpr double writingHz = 360.0;
    static constexpr double writingDelaySec = 1.0 / writingHz;

    // Windows, by default, has a clock interrupt rate that ticks every ~15.6 ms.
    // A Sleep() call can only complete on one of these ticks. If you request
    // a sleep of 1 ms, it will likely wait until the next tick occurs.
    //
    // That's why we use a "busy wait" instead for precise control of waiting time.
    static void PreciseSleep(double durationSeconds);

    static void Produce();
    static void Consume(std::function<void(float)> consumeItem);

public:
    static void ReadFile(const std::string& path);
    static void Start(std::function<void(float)> consumeItem);
    static void End();
};