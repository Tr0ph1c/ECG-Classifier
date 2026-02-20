#include "ecgdevicecapture.h"

#include "../miniaudio/miniaudio.cpp"

namespace ECGDeviceCapture {
const int SHAREDBUFFERSIZE = 4096;

float buffer[SHAREDBUFFERSIZE];
int bufferIndex = 0;
ma_device device;
std::mutex lock;
bool isRunning = false;

// practically the producer
// this gets called each time the device has data
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    lock.lock();

    for (ma_uint32 i = 0; i < frameCount; i++, bufferIndex++) {
        buffer[bufferIndex] = ((float*)pInput)[i];
    }

    lock.unlock();
}

void Produce() {
    ma_result result;
    ma_device_config deviceConfig;

    deviceConfig = ma_device_config_init(ma_device_type_capture);
    deviceConfig.capture.format   = ma_format_f32;
    deviceConfig.capture.channels = 0;
    deviceConfig.sampleRate       = 0;
    deviceConfig.dataCallback     = data_callback;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
        exit(-1);
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        exit(-2);
    }
}

void Consume(std::function<void(float)> consumeItem) {
    while (isRunning) {
        lock.lock();
        
        int currentCount = bufferIndex;
        for (int i = 0; i < currentCount; ++i) {
            consumeItem(buffer[i]);
        }
        bufferIndex = 0;
        
        lock.unlock();
    }
}

void Start(std::function<void(float)> consumeItem) {
    printf("Starting data stream");
    isRunning = true;
    
    std::thread producerThread(Produce);
    std::thread consumerThread(Consume, consumeItem);
    
    producerThread.detach();
    consumerThread.detach();
}

void End() {
    ma_device_uninit(&device);
    bufferIndex = 0;
    isRunning = false;
}
}