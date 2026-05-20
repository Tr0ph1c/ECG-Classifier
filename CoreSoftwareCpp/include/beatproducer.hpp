#pragma once

#include <vector>
#include <mutex>
#include "DetectorPanTompkins/realtime/DetectorPanTompkins.h"
#include "RingBuffer.hpp"
#include "Beat.hpp"

class BeatProducer
{
    private:
    int newR;
    int oldR;
    long processedSamples;
    DetectorPanTompkins detector = DetectorPanTompkins(360);
    
    public:
    int beatsize = 91;
    RingBuffer samplesBuffer = RingBuffer(beatsize);
    int iterationsToProduce;
    std::vector<Beat> detectedBeats;
    std::mutex mtx;

    BeatProducer(double);

    void process(double);
    void AddNewR(int r);
    void ProduceNewBeat();
};