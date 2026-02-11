#include "./SignalProducer.h"

std::vector<float> SignalProducer::fullRecording;
size_t SignalProducer::fullRecIndex = 0;
float SignalProducer::buffer[SHAREDBUFFERSIZE];
int SignalProducer::bufferIndex = 0;
std::mutex SignalProducer::lock;
bool SignalProducer::isRunning = false;

void SignalProducer::PreciseSleep(double durationSeconds) {
    auto start = std::chrono::high_resolution_clock::now();
    auto durationTicks = std::chrono::duration<double>(durationSeconds);
    
    while (std::chrono::high_resolution_clock::now() - start < durationTicks) {
        
    }
}

void SignalProducer::Produce() {
    while (isRunning) {
        lock.lock();
        
        size_t recIdx = std::min(fullRecording.size() - 1, fullRecIndex++);
        buffer[bufferIndex++] = fullRecording[recIdx];
        
        lock.unlock();

        PreciseSleep(writingDelaySec);
    }
}

void SignalProducer::Consume(std::function<void(float)> consumeItem) {
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

void SignalProducer::ReadFile(const std::string& path) {
    if (isRunning) End();
    fullRecording.clear();
    
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        try {
            fullRecording.push_back(std::stof(line));
        } catch (...) {
            printf("Invalid data line ( %s ) at file ( %s ).", line, path);
        }
    }
}

void SignalProducer::Start(std::function<void(float)> consumeItem) {
    printf("Starting data stream");
    isRunning = true;
    
    std::thread producerThread(Produce);
    std::thread consumerThread(Consume, consumeItem);
    
    producerThread.detach();
    consumerThread.detach();
}

void SignalProducer::End() {
    fullRecIndex = 0;
    bufferIndex = 0;
    isRunning = false;
}