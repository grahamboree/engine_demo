#pragma once

struct EngineState;
struct Renderer;

struct UIState
{
    bool showStatsWindow = true;
    bool showConsoleWindow = true;
    bool showImguiDemoWindow = false;

    void Init(const Renderer& renderer);
    void UpdateAndRender(const EngineState& engine);
};
