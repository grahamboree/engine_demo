#pragma once

#include "core.h"

struct Input {
    // mouse pos
    int mousePos[2] = {0, 0};
    int mouseDelta[2] = {0, 0};

    int mouseScroll = 0;

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

    bool mouseOverUI = false;

    // keyboard
    bool left = false;
    bool right = false;
    bool forward = false;
    bool back = false;
    bool up = false;
    bool down = false;

    void ProcessSDLEvent(SDL_Event* event) {
        switch (event->type) {
            case SDL_MOUSEMOTION: {
                mouseDelta[0] = event->motion.xrel;
                mouseDelta[1] = -event->motion.yrel;
            } break;
            case SDL_MOUSEWHEEL: {
                if (event->wheel.y < 0) {
                    // wheel down
                    mouseScroll++;
                } else {
                    mouseScroll--;
                }
            } break;
        }
    }

    void readInput() {
        mouseDelta[0] = 0;
        mouseDelta[1] = 0;
        Uint32 buttonState = SDL_GetMouseState(&mousePos[0], &mousePos[1]);

        bool newMouseLeft = buttonState & SDL_BUTTON(SDL_BUTTON_LEFT);
        bool newMouseRight = buttonState & SDL_BUTTON(SDL_BUTTON_RIGHT);
        bool newMouseMiddle = buttonState & SDL_BUTTON(SDL_BUTTON_MIDDLE);

        mouseDownLeft = newMouseLeft && !mouseLeft;
        mouseDownRight = newMouseRight && !mouseRight;
        mouseDownMiddle = newMouseMiddle && !mouseMiddle;

        mouseUpLeft = !newMouseLeft && mouseLeft;
        mouseUpRight = !newMouseRight && mouseRight;
        mouseUpMiddle = !newMouseMiddle && mouseMiddle;

        mouseLeft = newMouseLeft;
        mouseRight = newMouseRight;
        mouseMiddle = newMouseMiddle;

        mouseOverUI = ImGui::GetIO().WantCaptureMouse;

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        left = keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT];
        right = keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT];
        up = keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP];
        down = keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN];
    }
};
