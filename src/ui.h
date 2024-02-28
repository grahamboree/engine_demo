#pragma once

struct Camera;
struct Stats;
struct Renderer;

struct UIState
{
    bool showStatsWindow = true;
    bool showConsoleWindow = true;
    bool showImguiDemoWindow = false;

    void Init(const Renderer& renderer);
    void UpdateAndRender(const Renderer& renderer, const Stats& stats, const Camera& camera);
};