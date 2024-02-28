#pragma once

#include "core.h"
#include "input.h"

struct Camera {
    static constexpr float nearPlane = 0.1f;
    static constexpr float farPlane = 1000;
    
    glm::vec3 pos;
    glm::vec2 euler;
    int viewportWidth;
    int viewportHeight;
    glm::mat4 projectionMatrix;

    void SetOrtho(float width);
    glm::mat4 CalculateViewMatrix();
};
