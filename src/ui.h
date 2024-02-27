#pragma once

struct Camera;
struct Stats;
struct RendererState;

struct UIState
{
    bool showStatsWindow = true;
    bool showConsoleWindow = true;
    bool showImguiDemoWindow = false;

    void Init(const RendererState& renderer);
    void UpdateAndRender(const RendererState& renderer, const Stats& stats, const Camera& camera);
};