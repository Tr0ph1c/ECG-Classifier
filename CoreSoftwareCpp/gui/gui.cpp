#include "gui.hpp"
#include <vector>
#include <iostream>

namespace gui
{
    RingBuffer buffer(1000);
    bool running = false;
    std::vector<float> plotData;
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;

    void init()
    {
        // ===== SDL Init =====
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
        {
            printf("SDL Init failed: %s\n", SDL_GetError());
            exit(-1);
        }

        window = SDL_CreateWindow("Realtime ECG Wave",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1000,
            600,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        // ===== ImGui Init =====
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer2_Init(renderer);
        running = true;

    }

    void shutdown()
    {
        ImGui_ImplSDLRenderer2_Shutdown();
        ImGui_ImplSDL2_Shutdown();

        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }

    void new_frame()
    {
               // ===== Events =====
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
                running = false;
        }

        // ===== Copy data from ring buffer (thread-safe) =====
        buffer.copy(plotData);

        // ===== ImGui Frame =====
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // Make ImGui window fill the entire SDL window
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Realtime Signal", nullptr,
                     ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoTitleBar);

        if (ImPlot::BeginPlot("ECG Wave", ImVec2(-1, -1)))
        {
            if (!plotData.empty())
            {
                ImPlot::PlotLine(
                    "Signal",
                    plotData.data(),
                    plotData.size());
            }
            ImPlot::EndPlot();
        }

        ImGui::End();

        // ===== Render =====
        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // IMPORTANT: New API requires renderer param
        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

}