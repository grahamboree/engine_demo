#pragma once

union SDL_Event;

struct Input {
    int mouseScroll = 0;
    bool mouseOverUI = false;

    // mouse pos
    int mousePos[2] = {0, 0};
    int mouseDelta[2] = {0, 0};

    // mouse buttons
    bool mouseLeft = false;
    bool mouseDownLeft = false;
    bool mouseUpLeft = false;

    bool mouseRight = false;
    bool mouseDownRight = false;
    bool mouseUpRight = false;

    bool mouseMiddle = false;
    bool mouseDownMiddle = false;
    bool mouseUpMiddle = false;

    // keyboard
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    void ProcessSDLEvent(SDL_Event* event);
    void ReadInput();
};
