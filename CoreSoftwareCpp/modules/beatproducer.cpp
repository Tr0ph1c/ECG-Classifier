#include "DetectorPanTompkins/realtime/DetectorPanTompkins.h"
#include "beatproducer.hpp"
    
BeatProducer::BeatProducer(double fs)
{
    newR = -1;
    oldR = -1;
    processedSamples = 0;
    iterationsToProduce = -1;
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

    // send beat to be classified
    beat.beatClass = 'N'; // temporary N classification on all beats

    detectedBeats.push_back(beat);
}