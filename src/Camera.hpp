#pragma once
#include "../include/glm/glm.hpp"
class Camera{
public:
    Camera();
    Camera(float pitch, float yaw, glm::vec3 lookat);
    void Rotate(float deltaYaw, float deltaPitch);
    glm::vec3 position, right, lookat;
private:
    float yaw, pitch;
};