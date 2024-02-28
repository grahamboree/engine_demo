#include "stats.h"

#include <cstring>

using floatMillis = std::chrono::duration<float, std::chrono::milliseconds::period>;

namespace {
void UpdateRecordedStat(float* history, int historySize, float newValue) {
    // Shift over frame time history and add the newest value to the end.
    for (int i = 0; i < historySize - 1; ++i) {
        history[i] = history[i + 1];
    }
    history[historySize - 1] = newValue;
}
}


Stats::Stats() {
    memset(frameTimesMillis, 0, sizeof(float) * FRAME_TIME_HISTORY_SIZE);
    memset(updateTimesMillis, 0, sizeof(float) * FRAME_TIME_HISTORY_SIZE);
    memset(renderTimesMillis, 0, sizeof(float) * FRAME_TIME_HISTORY_SIZE);
}

void Stats::RecordFrameTime(floatMillis frameTime) {
    UpdateRecordedStat(frameTimesMillis, FRAME_TIME_HISTORY_SIZE, frameTime.count());
}

void Stats::RecordUpdateTime(floatMillis updateTime) {
    UpdateRecordedStat(updateTimesMillis, FRAME_TIME_HISTORY_SIZE, updateTime.count());
}

void Stats::RecordRenderTime(floatMillis renderTime) {
    UpdateRecordedStat(renderTimesMillis, FRAME_TIME_HISTORY_SIZE, renderTime.count());
}