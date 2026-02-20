#pragma once
#include <fstream>
#include <vector>
#include <mutex>
#include <functional>
#include <thread>

namespace ECGDeviceCapture {
    void Start(std::function<void(float)> consumeItem);
    void End();
}