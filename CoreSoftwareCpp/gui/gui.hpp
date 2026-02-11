#pragma once

#include "../modules/RingBuffer.hpp"

#include <SDL2/SDL.h>

// ImGui
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

// ImPlot
#include "implot.h"

namespace gui
{
    extern RingBuffer buffer;
    extern bool running;
    void init();
    void shutdown();
    void new_frame();
    void render();
}