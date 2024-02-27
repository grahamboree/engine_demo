#pragma once

#include <vector>
#include <string>

extern std::vector<std::string> consoleScrollback;

enum class LogCategory {
    PERF,
    LUA,
    GFX
};

void log(LogCategory category, const char* format, ...);
void log_perf_counter(uint64_t start, uint64_t end, const char* name);
