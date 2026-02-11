#include <iostream>
#include "modules/SignalProducer.h"
#include "gui/gui.hpp"

void process (float f) {
    gui::buffer.push(f);
}

int main (int argv, char** args) {
    SignalProducer::ReadFile("./100_raw");
    SignalProducer::Start(process);
    gui::init();
    while (gui::running)
    {
        gui::new_frame();
    }
    
    gui::shutdown();
}