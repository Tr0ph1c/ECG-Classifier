#pragma once

#include "RingBuffer.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <mutex>

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

// ImPlot
#include "implot.h"

#include "Beat.hpp"
#include "beatproducer.hpp"

namespace gui
{
    extern std::vector<double> plotData;
    extern bool running;
    void init();
    void shutdown();
    void SetBeatsList(BeatProducer*);
    void new_frame();
    void render_beat(Beat);
    void render();
}