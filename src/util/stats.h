#pragma once

#include <chrono>

struct Stats {
    using floatMillis = std::chrono::duration<float, std::chrono::milliseconds::period>;

    // 3 seconds at 60 fps
    static constexpr int FRAME_TIME_HISTORY_SIZE = 60 * 3;

    float frameTimesMillis[FRAME_TIME_HISTORY_SIZE];
    float updateTimesMillis[FRAME_TIME_HISTORY_SIZE];
    float renderTimesMillis[FRAME_TIME_HISTORY_SIZE];

    Stats();
    void RecordFrameTime(floatMillis frameTime);
    void RecordUpdateTime(floatMillis updateTime);
    void RecordRenderTime(floatMillis renderTime);
};
