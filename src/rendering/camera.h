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

    void SetPerspective() {
        projectionMatrix = glm::perspective(
            glm::radians(fov),
            aspect(),
            near,
            far
        );
    }

    void SetOrtho(float width) {
        float height = width / aspect();
        float halfHeight = height / 2.0f;
        float halfWidth = width / 2.0f;
        projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, near, far);
    }

    float aspect() const { return (float)viewportWidth / viewportHeight; }

    void move(float dt, const Input& input, SDL_Window* window, float windowHeight) {
        glm::mat4 viewMatrix = glm::eulerAngleXY(euler.x, euler.y);
        
        if (input.mouseDownLeft) {
            lastMouseDownPos[0] = input.mousePos[0];
            lastMouseDownPos[1] = input.mousePos[1];
            SDL_SetRelativeMouseMode(SDL_TRUE);
        } else if (input.mouseUpLeft) {
            SDL_SetRelativeMouseMode(SDL_FALSE);
            SDL_WarpMouseInWindow(window, lastMouseDownPos[0], lastMouseDownPos[1]);
        } else if (input.mouseLeft) {
            // 1440x900 origin top left
            const float screenPlaneDistance = (windowHeight / 2.0f) / std::tan(glm::radians(fov / 2));
            euler.y += std::atan2(static_cast<float>(input.mouseDelta[0]), screenPlaneDistance);
            euler.x -= std::atan2(static_cast<float>(input.mouseDelta[1]), screenPlaneDistance);
            euler.x = clamp(euler.x, glm::radians(-90.0f), glm::radians(90.0f));
        }
        
        // Handle keyboard movement.
        float accel = 4 * dt;
        moveFront = clamp01(moveFront + (input.forward ? accel : -accel));
        moveLeft  = clamp01(moveLeft  + (input.left    ? accel : -accel));
        moveBack  = clamp01(moveBack  + (input.back    ? accel : -accel));
        moveRight = clamp01(moveRight + (input.right   ? accel : -accel));
        moveUp    = clamp01(moveUp    + (input.up      ? accel : -accel));
        moveDown  = clamp01(moveDown  + (input.down    ? accel : -accel));
        
        float moveSpeed = 22.0f;
        if (SDL_GetModState() & KMOD_SHIFT) {
            moveSpeed *= 4.0f;
        }

        float movex = (moveRight - moveLeft) * moveSpeed * dt;
        float movey = (moveBack - moveFront) * moveSpeed * dt;
        
        pos[0] += movex * (float)viewMatrix[0][0];
        pos[1] += movex * (float)viewMatrix[1][0];
        pos[2] += movex * (float)viewMatrix[2][0];

        pos[0] += movey * (float)viewMatrix[0][2];
        pos[1] += movey * (float)viewMatrix[1][2];
        pos[2] += movey * (float)viewMatrix[2][2];

        pos[1] += (moveUp - moveDown) * moveSpeed * dt;
    }

    glm::mat4 CalculateViewProjectionMatrix() {
        glm::mat4 viewMatrix = glm::eulerAngleXY(euler.x, euler.y) * glm::translate(glm::mat4(1.0f), -pos);
        return projectionMatrix * viewMatrix;
    }
};
