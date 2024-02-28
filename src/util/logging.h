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
