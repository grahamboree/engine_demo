#include "camera.h"

void Camera::SetOrtho(float width) {
    float aspect = (float)viewportWidth / viewportHeight;
    float height = width / aspect;
    float halfHeight = height / 2.0f;
    float halfWidth = width / 2.0f;
    projectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
}

glm::mat4 Camera::CalculateViewProjectionMatrix() {
    glm::mat4 viewMatrix = glm::eulerAngleXY(euler.x, euler.y) * glm::translate(glm::mat4(1.0f), -pos);
    return projectionMatrix * viewMatrix;
}