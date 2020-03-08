#include "header.h"

Entity::Entity(glm::vec3 position, float rotationY){
    this->transformationMatrix = glm::mat4(1.0f);
    this->SetPosition(position);
    this->path = {this->tileIndex, this->tileIndex};
    this->SetRotation(rotationY);
}
void Entity::ChangePosition(double delta_time, const std::unordered_set<uint>* blockedTiles){
// move the entity towards its goal, depending on the time that passed
// check before each step if the next tile is blocked
// if it is, do something...
    glm::vec3 goal_position = {this->GetCurrentGoalIndex().x * TILE_SIZE, 0.0f, this->GetCurrentGoalIndex().y * TILE_SIZE};
    glm::vec3 delta_pos = goal_position - this->position;
    glm::vec3 newPosition;
    if(glm::length(delta_pos) >= TILE_SIZE / 20.0){
        delta_pos = float(delta_time) * this->speed * glm::normalize(delta_pos);
        newPosition = this->position + delta_pos;
    }
    else{
        newPosition = goal_position;
    }
    glm::ivec2 newTile = {glm::ceil(newPosition.x / TILE_SIZE - 0.5f), glm::ceil(newPosition.z / TILE_SIZE - 0.5f)};
    if(blockedTiles->find(newTile.x * NUM_GRID + newTile.y) == blockedTiles->end()){
        this->position = newPosition;
        this->tileIndex = newTile;
    }
    else{
        this->SetGoalIndex(this->tileIndex);
    }

    this->transformationMatrix = glm::translate(glm::mat4(1.0f), this->position);
}
void Entity::SetRotation(float newRotation){
    this->transformationMatrix = glm::translate(glm::mat4(1.0f), this->position) * 
                                 glm::rotate(newRotation, glm::vec3(0.0, 1.0, 0.0));
                        
    this->rotationY = newRotation;
}

void Entity::SetPosition(glm::vec3 newPosition){
    this->transformationMatrix = glm::rotate(this->rotationY, glm::vec3(0.0, 1.0, 0.0));
    this->transformationMatrix = glm::translate(this->transformationMatrix, newPosition);
    this->position = newPosition;
    this->tileIndex.x = glm::ceil(this->position.z / TILE_SIZE - 0.5f);
    this->tileIndex.y = glm::ceil(this->position.x / TILE_SIZE - 0.5f);
}
void Entity::SetScale(float newScale){
    this->transformationMatrix *= glm::scale(glm::vec3(1.0f, newScale, 1.0f));
}
float* Entity::GetTransformationMatrix(){
    return &(this->transformationMatrix[0][0]);
}

glm::ivec2 Entity::GetCurrentGoalIndex(){
    return this->path.GetGoalIndex();
}

void Entity::SetGoalIndex(glm::ivec2 goal){
    glm::vec3 goalPosition = glm::vec3(goal.x * TILE_SIZE, 0.0f, goal.y * TILE_SIZE);
    this->currentDirection = glm::normalize(goalPosition - this->position);
    this->path.Set(this->tileIndex, goal);
}
glm::ivec2 Entity::GetTileIndex(){
    return this->tileIndex;
}