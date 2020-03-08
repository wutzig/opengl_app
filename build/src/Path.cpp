#include "header.h"
Path::Path():
startIndex(0), goalIndex(0){
}
Path::Path(glm::ivec2 startIndex, glm::ivec2 goalIndex):
startIndex(startIndex), goalIndex(goalIndex){

    int deltaX = goalIndex.x - startIndex.x;
    int deltaY = goalIndex.y - startIndex.y;

    if(deltaX == 0.0f) this->direction = 0.0f;
    else this->direction = float(deltaY) / float(deltaX);
}

float Path::GetDirection(){
    return this->direction;
}

void Path::Set(glm::ivec2 startIndex, glm::ivec2 goalIndex){
    this->startIndex = startIndex;
    this->goalIndex = goalIndex;

    int deltaX = goalIndex.x - startIndex.y;
    int deltaY = goalIndex.y - startIndex.y;

    if(deltaX == 0.0f) this->direction = 0.0f;
    else this->direction = float(deltaY) / float(deltaX);
}

glm::ivec2 Path::GetGoalIndex(){
    return this->goalIndex;
}