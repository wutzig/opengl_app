#include "Camera.hpp"
#include "Renderer.hpp"

Camera::Camera():
position(glm::vec3(0.0f, 0.0f, -1.0f)), right(glm::vec3(1.0f, 0.0f, 0.0f)), lookat(glm::vec3(0.0f)){
    this->yaw = 0.0f;
    this->pitch = 0.0f;
}
Camera::Camera(float pitch, float yaw, glm::vec3 lookat):
pitch(pitch), yaw(yaw), lookat(lookat){
    this->Rotate(0.0f, 0.0f);
    
}

void Camera::Rotate(float deltaYaw, float deltaPitch){
    this->pitch = glm::min(glm::max(this->pitch + deltaPitch, PI / 12.0f), 5.0f * PI / 12.0f);
    this->yaw += deltaYaw;
    this->right = Renderer::quatRot(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), this->yaw, glm::vec3(0.0f));
    this->position = Renderer::quatRot(glm::vec3(0.0f, 0.0f, -22.0f), glm::vec3(0.0f, 1.0f, 0.0f), this->yaw, glm::vec3(0.0f));
    this->position = Renderer::quatRot(this->position, this->right, this->pitch, glm::vec3(0.0f));
    this->position += this->lookat;
}