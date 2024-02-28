#include "engine.h"

EngineState engine;

int main(int argv, char* args[]) {
    engine.Init();

    // main loop
    while (!engine.UpdateAndRender()) {}

    engine.Shutdown();

    return 0;
}
