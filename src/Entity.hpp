#pragma once
#include "../include/glm/glm.hpp"
#include <unordered_set>
#include "Path.hpp"
class Entity{
public:
    Entity(glm::vec3 position = glm::vec3(0.0f), float rotationY = 0.0f);
    void SetRotation(float newRotation);
    void SetPosition(glm::vec3 newPosition);
    void SetScale(float newScale);
    float* GetTransformationMatrix();
    void ChangePosition(double delta_time, const std::unordered_set<int>* blockedTiles);
    glm::ivec2 GetTileIndex();
    glm::ivec2 GetCurrentGoalIndex();
    void SetGoalIndex(glm::ivec2 goal);
private:
    glm::mat4 transformationMatrix;
    Path path;
    float rotationY, speed = 2.0f;
    glm::vec3 position, currentDirection;
    glm::ivec2 tileIndex;
};