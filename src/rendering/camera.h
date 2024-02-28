#pragma once

#include "core.h"
#include "input.h"

struct Camera {
    glm::vec3 pos;
    glm::vec2 euler;

    float near = 0.1f;
    float far = 1000;
    float fov = 50;

    int viewportWidth;
    int viewportHeight;

    // Movement velocities
    float moveFront = 0.0f;
    float moveLeft  = 0.0f;
    float moveBack  = 0.0f;
    float moveRight = 0.0f;
    float moveUp    = 0.0f;
    float moveDown  = 0.0f;

    int lastMouseDownPos[2];

    glm::mat4 projectionMatrix;

    void SetPerspective();
    void SetOrtho(float width);

    float aspect() const { return (float)viewportWidth / viewportHeight; }

    void move(float dt, const Input& input, SDL_Window* window, float windowHeight);
    glm::mat4 CalculateViewProjectionMatrix();
};
