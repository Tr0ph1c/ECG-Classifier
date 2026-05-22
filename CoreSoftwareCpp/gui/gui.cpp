#include "gui.hpp"

namespace gui
{
    //RingBuffer buffer(2000);
    bool running = false;
    std::vector<double> plotData;
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    BeatProducer *beatProducer = nullptr;

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

    void SetBeatsList(BeatProducer* producer) {
        beatProducer = producer;
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
        //buffer.copy(plotData);

        // ===== ImGui Frame =====
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        // Make ImGui window fill the entire SDL window
        ImGui::Begin("Dashboard", nullptr,
                     ImGuiWindowFlags_NoResize
                         | ImGuiWindowFlags_NoMove
                         | ImGuiWindowFlags_NoCollapse
                         /*| ImGuiWindowFlags_NoTitleBar*/);

        
        if (ImGui::BeginTabBar("Tabs")) 
        {
            if (ImGui::BeginTabItem("Full Recording View")) 
            {
                if (ImPlot::BeginPlot("ECG Wave", ImVec2(-1, -1))) 
                {
                    if (!plotData.empty()) 
                    {
                        ImPlot::PlotLine(
                            "Signal",
                            plotData.data(),
                            plotData.size());
                    }

                    // Show beat class visually on top of each beat.
                    for (Beat b : beatProducer->detectedBeats) {
                        ImPlot::PlotText(&b.beatClass, b.index, 1.25);
                    }

                    ImPlot::EndPlot();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Beats")) 
            {
                std::vector<Beat> beatsCopy;

                {
                        std::lock_guard<std::mutex> lock(beatProducer->mtx);
                        beatsCopy = beatProducer->detectedBeats;
                }

                // TODO: Add percentage of "Non Normal" beats as a statistic on top 
                //ImGui::Text("Normal Beats: %d", beatsCopy.)

                ImGui::BeginChild("BeatScrollArea", ImVec2(0, 0), true);

                for (const Beat& b : beatsCopy)
                {
                    ImGui::PushID(&b);

                    // Visual block for each beat
                    ImGui::BeginChild("BeatBlock", ImVec2(0, 140), true);

                    // Mini plot for beat's data
                    if (ImPlot::BeginPlot("##", ImVec2(250, 120), ImPlotFlags_NoLegend))
                    {
                        ImPlot::SetupAxes(nullptr, nullptr,
                            ImPlotAxisFlags_NoTickLabels,
                            ImPlotAxisFlags_NoTickLabels);

                        ImPlot::PlotLine(
                            "",
                            b.samples.data(),
                            beatProducer->beatsize
                        );

                        ImPlot::EndPlot();
                    }

                    ImGui::SameLine();

                    // Beat's class
                    ImGui::BeginGroup();

                    ImGui::Text("Class:");
                    ImGui::Text("%c", (char)b.beatClass);

                    ImGui::Spacing();

                    ImGui::EndGroup();

                    ImGui::EndChild();

                    ImGui::Spacing();

                    ImGui::PopID();
                }

                ImGui::EndChild();

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();

        // ===== Render =====
        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer2_RenderDrawData(
            ImGui::GetDrawData(),
            renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}