#include <iostream>
#include "ecgdevicecapture.h"
#include "gui.hpp"

void process (float f) {
    gui::buffer.push(f);
}

int main (int argv, char** args) {
    ECGDeviceCapture::Start(process);
    gui::init();
    while (gui::running)
    {
        gui::new_frame();
    }
    
    gui::shutdown();
    return 0;
}