#include "io.h"

#include <cstdio>

char* IO::ReadFile(const char* path) {
#if _WIN32
    FILE* vert;
    if (fopen_s(&vert, path, "rb")) {
        vert = nullptr;
    }
#else
    FILE* vert = fopen(path, "rb");
#endif
    
    if (!vert) {
        return nullptr;
    }

    if (fseek(vert, 0, SEEK_END) != 0) {
        fclose(vert);
        return nullptr;
    }

    const auto bufSize = ftell(vert);
    if (bufSize < 0) {
        fclose(vert);
        return nullptr;
    }

    char* buf = new char[bufSize + 1];

    if (fseek(vert, 0, SEEK_SET) != 0) {
        fclose(vert);
        delete[] buf;
        return nullptr;
    }

    const size_t readLen = fread(buf, 1, bufSize, vert);
    fclose(vert);

    if (readLen != bufSize) {
        delete[] buf;
        return nullptr;
    }
    
    buf[bufSize] = '\0';
    return buf;
}
