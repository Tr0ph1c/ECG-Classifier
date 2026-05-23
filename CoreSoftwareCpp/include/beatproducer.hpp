#pragma once

#include <vector>
#include <mutex>
#include <windows.h>
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
    HANDLE* pipe;
    
    public:
    int beatsize = 91;
    RingBuffer samplesBuffer = RingBuffer(beatsize);
    std::vector<Beat> detectedBeats;
    std::mutex mtx;

    BeatProducer(double, HANDLE*);

    void process(double);
    void AddNewR(int r);
    void ProduceNewBeat();
    char ClassifyBeat(std::vector<double>);
};