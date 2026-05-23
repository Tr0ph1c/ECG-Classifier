#include "DetectorPanTompkins/realtime/DetectorPanTompkins.h"
#include "beatproducer.hpp"
    
BeatProducer::BeatProducer(double fs, HANDLE* _pipe)
{
    newR = -1;
    oldR = -1;
    processedSamples = 0;

    pipe = _pipe;
}

void BeatProducer::process(double sample)
{
    processedSamples++;
    samplesBuffer.push(sample);

    int result = detector.processSample(samplesBuffer.chronoIndex((int)beatsize/2)); // process from the middle of the buffer [------QRS------]

    if (result != -1) {
        AddNewR(result);
        ProduceNewBeat();
    }
}

void BeatProducer::AddNewR(int r)
{
    oldR = newR;
    newR = r;
}

void BeatProducer::ProduceNewBeat()
{
    std::lock_guard<std::mutex> lock(mtx);

    Beat beat;
    samplesBuffer.copy(beat.samples);
    beat.rrInterval = newR - oldR;
    beat.index = newR;
    
    // build the features vector
    std::vector<double> features(beatsize + 1);
    features = beat.samples;
    features.back() = beat.rrInterval;

    beat.beatClass = ClassifyBeat(features);

    detectedBeats.push_back(beat);
}

char BeatProducer::ClassifyBeat(std::vector<double> features)
{
    // send features length
    
    DWORD bytesWritten;
    unsigned int signalLength = static_cast<unsigned int>(features.size());

    WriteFile(
        *pipe,
        &signalLength,
        sizeof(signalLength),
        &bytesWritten,
        NULL
    );

    // send signal

    WriteFile(
        *pipe,
        features.data(),
        signalLength * sizeof(float),
        &bytesWritten,
        NULL
    );

    // receive classification

    char buffer[8] = {};
    DWORD bytesRead;

    ReadFile(
        *pipe,
        buffer,
        sizeof(buffer) - 1,
        &bytesRead,
        NULL
    );

    return buffer[0];
}