#include "io.h"

#include <cstdio>

char* IO::ReadFile(const char* path) {
#if _WIN32
    FILE* file;
    if (fopen_s(&file, path, "rb")) {
        file = nullptr;
    }
#else
    FILE* file = fopen(path, "rb");
#endif
    
    if (!file) {
        return nullptr;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return nullptr;
    }

    const auto bufSize = ftell(file);
    if (bufSize < 0) {
        fclose(file);
        return nullptr;
    }

    char* buf = new char[bufSize + 1];

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        delete[] buf;
        return nullptr;
    }

    const size_t readLen = fread(buf, 1, bufSize, file);
    fclose(file);

    if (readLen != bufSize) {
        delete[] buf;
        return nullptr;
    }
    
    buf[bufSize] = '\0';
    return buf;
}
