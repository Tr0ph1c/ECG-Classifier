#include <iostream>
#include "SignalProducer.h"
#include "ecgdevicecapture.h"
#include "beatproducer.hpp"
#include "gui.hpp"

BeatProducer beatProducer(360);

void hint () {
    printf("Use: detector [demo/device]\nRunning ecgcd without arguments will run it in demo mode by default.\nMake sure the ECG capture device is connected before running in device mode.\n");
}

void process (float f) {
    //gui::buffer.push(f);
    gui::plotData.push_back(f);
    beatProducer.process(f);
}

int main (int argv, char** args) {
    SignalProducer::ReadFile("./100_raw");
    gui::SetBeatsList(&beatProducer);

    switch (argv) {
        case 1:
            SignalProducer::Start(process);
            break;
        case 2:
            if (args[1] == "demo") {
                SignalProducer::Start(process);
            } else if (args[1] == "device") {
                ECGDeviceCapture::Start(process);
            } else {
                printf("%s is not a supported mode. Only (demo, device) are supported.", args[1]);
                exit(0);
            }
            break;
        default:
            hint();
            exit(0);
    }
    
    gui::init();
    while (gui::running) gui::new_frame();
    gui::shutdown();
    return 0;
}