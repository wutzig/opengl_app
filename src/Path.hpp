#pragma once
#include <vector>
#include "../include/glm/glm.hpp"
class Path{
public:
    Path(glm::ivec2 startIndex, glm::ivec2 goalIndex);
    Path();
    float GetDirection();
    glm::ivec2 GetGoalIndex();
    void Set(glm::ivec2 startIndex, glm::ivec2 goalIndex);

private:
    glm::ivec2 startIndex, goalIndex;
    std::vector<uint> tiles;
    float direction;
};