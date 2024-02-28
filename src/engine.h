#pragma once

#include <chrono>

#include "core.h"
#include "rendering/camera.h"
#include "rendering/gfx.h"
#include "entity.h"
#include "input.h"
#include "lua_api.h"
#include "ui.h"
#include "util/stats.h"

struct EngineState {
    // Config
    static constexpr int windowWidth = 1440;
    static constexpr int windowHeight = 900;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameStart;
    std::chrono::time_point<std::chrono::high_resolution_clock> gameStartTime;

    // Subsystems
    Stats stats;
    Input input;
    Camera camera;
    LuaRuntime lua;
    Renderer renderer;
    EntityState entities;
    UIState ui;

    void Init();
    bool UpdateAndRender(); // Returns true if we should exit.
    void Shutdown();
};

