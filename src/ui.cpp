#include "ui.h"

#include "util/stats.h"
#include "core.h"
#include "rendering/gfx.h"
#include "rendering/camera.h"

void UIState::Init(const RendererState& renderer) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigWindowsResizeFromEdges = true;
    io.Fonts->AddFontFromFileTTF(DATA_PATH "Fonts/Cousine-Regular.ttf", 14);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(renderer.window, renderer.glContext);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void UIState::UpdateAndRender(const RendererState& renderer, const Stats& stats, const Camera& camera) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(renderer.window);
    ImGui::NewFrame();

    // stats window
    if (showStatsWindow) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        auto size = viewport->WorkSize;
        size.x *= 0.25f;
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.3f, 0.3f, 0.32f, 0.4f });

        ImGui::Begin("Stats", &showStatsWindow, ImGuiWindowFlags_NoDocking |
                                                ImGuiWindowFlags_NoTitleBar |
                                                ImGuiWindowFlags_NoCollapse |
                                                ImGuiWindowFlags_NoResize |
                                                ImGuiWindowFlags_NoMove |
                                                ImGuiWindowFlags_NoBringToFrontOnFocus |
                                                ImGuiWindowFlags_NoNavFocus);
        {
            ImGui::PopStyleVar(3); // Window rounding, window border size, window padding
            ImGui::PopStyleColor(1); // Window background color

            const auto title = "Stats";
            ImGui::SetCursorPosX((size.x - ImGui::CalcTextSize(title).x) * 0.5f);
            ImGui::Text(title);

            { // Plot frame times.
                float avgUpdateTime = 0;
                float avgRenderTime = 0;
                float avgFrameTime = 0;
                float maxFrameTime = 0;

                for (int i = 0; i < Stats::FRAME_TIME_HISTORY_SIZE; ++i) {
                    const float frameTime = stats.frameTimesMillis[i];
                    const float updateTime = stats.updateTimesMillis[i];
                    const float renderTime = stats.renderTimesMillis[i];

                    avgFrameTime += frameTime;
                    avgUpdateTime += updateTime;
                    avgRenderTime += renderTime;

                    if (frameTime > maxFrameTime) {
                        maxFrameTime = frameTime;
                    }
                }
                avgFrameTime /= Stats::FRAME_TIME_HISTORY_SIZE;
                avgUpdateTime /= Stats::FRAME_TIME_HISTORY_SIZE;
                avgRenderTime /= Stats::FRAME_TIME_HISTORY_SIZE;

                char avgLabel[128];
                sprintf(avgLabel, "avg f:%.2f u:%.2f r:%.2f", avgFrameTime, avgUpdateTime, avgRenderTime);

                float maxY = std::max<float>(maxFrameTime, 16);

                ImPlot::SetNextPlotLimitsX(0, Stats::FRAME_TIME_HISTORY_SIZE - 1, ImGuiCond_Always);
                ImPlot::SetNextPlotLimitsY(0, maxY, ImGuiCond_Always);
                ImPlot::PushStyleColor(ImPlotCol_FrameBg, ImVec4{ 0, 0, 0, 0 });
                static constexpr ImPlotAxisFlags X_AXIS_FLAGS = ImPlotAxisFlags_Lock | ImPlotAxisFlags_NoGridLines | ImPlotAxisFlags_NoTickMarks | ImPlotAxisFlags_NoTickLabels;
                static constexpr ImPlotAxisFlags Y_AXIS_FLAGS = ImPlotAxisFlags_Lock;
                if (ImPlot::BeginPlot("##FrameTimes", avgLabel, "Frame Time (ms)", ImVec2(-1, 150), 0, X_AXIS_FLAGS, Y_AXIS_FLAGS)) {
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::PlotShaded("frame", stats.frameTimesMillis, Stats::FRAME_TIME_HISTORY_SIZE);
                    ImPlot::PlotShaded("update", stats.updateTimesMillis, Stats::FRAME_TIME_HISTORY_SIZE);
                    ImPlot::PlotShaded("render", stats.renderTimesMillis, Stats::FRAME_TIME_HISTORY_SIZE);
                    ImPlot::PopStyleVar();

                    ImPlot::PlotLine("frame", stats.frameTimesMillis, Stats::FRAME_TIME_HISTORY_SIZE);
                    ImPlot::PlotLine("update", stats.updateTimesMillis, Stats::FRAME_TIME_HISTORY_SIZE);
                    ImPlot::PlotLine("render", stats.renderTimesMillis, Stats::FRAME_TIME_HISTORY_SIZE);

                }
                ImPlot::EndPlot();
                ImPlot::PopStyleColor(1);
            }

            ImGui::Text("Camera");
            ImGui::Indent();
            ImGui::Text("pos: x%.2f y%.2f z%.2f", camera.pos.x, camera.pos.y, camera.pos.z);
            ImGui::Text("rot: x%.2f y%.2f", camera.euler.x, camera.euler.y);
            ImGui::Unindent();

        }
        ImGui::End();
    }

    // Console Window
    if (showConsoleWindow) {
        ImGui::Begin("Console", &showConsoleWindow, ImGuiWindowFlags_NoCollapse);
        ImGui::BeginChild("Scrolling");
        for (const auto& line : consoleScrollback) {
            ImGui::TextUnformatted(line.c_str());
        }
        static size_t lastLineLength = 0;
        if (lastLineLength != consoleScrollback.size()) {
            ImGui::SetScrollHereY();
            lastLineLength = consoleScrollback.size();
        }
        ImGui::EndChild();
        ImGui::End();
    }

    if (showImguiDemoWindow) {
        ImGui::ShowDemoWindow(&showImguiDemoWindow);
        ImPlot::ShowDemoWindow();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore
    // it to make it easier to paste this code elsewhere.
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        SDL_Window* currentWindow = SDL_GL_GetCurrentWindow();
        const SDL_GLContext currentContext = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(currentWindow, currentContext);
    }
}
