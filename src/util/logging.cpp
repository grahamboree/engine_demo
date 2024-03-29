#include "logging.h"

#include "core.h"

std::vector<std::string> consoleScrollback;

void log(LogCategory category, const char* format, ...) {
    static constexpr size_t buffSize = 2048;
    static char buff[buffSize];

    uint32_t startPos = 0;
    switch (category) {
        case LogCategory::PERF:
            snprintf(buff, buffSize, "[  PERF ] ");
            startPos = 10;
            break;
        case LogCategory::LUA:
            snprintf(buff, buffSize, "[  LUA  ] ");
            startPos = 10;
            break;
        case LogCategory::GFX:
            snprintf(buff, buffSize, "[  GFX  ] ");
            startPos = 10;
            break;
    }
    
    va_list args;
    va_start(args, format);
#if _WIN32
    vsprintf_s(&buff[startPos], std::size(buff), format, args);
#else
    vsnprintf(&buff[startPos], buffSize - startPos, format, args);
#endif
    printf("%s\n", buff);
    va_end(args);

    consoleScrollback.emplace_back(buff);
}
