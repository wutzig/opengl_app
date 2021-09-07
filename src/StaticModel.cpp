#include "Model.hpp"
#include "../include/GL/glew.h"
#include "App.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/transform.hpp"
#include <iostream>
StaticModel::StaticModel(const int& numQuads, float *vertices, float *normals, float *colors, int numInstances, float* positions, float* angles):
numInstances(numInstances), Model(numQuads, vertices, normals, colors)
{
    std::cout << "calling static model constructor\n";
    this->instanceMatrices = new glm::mat4[numInstances];
    for(uint j = 0; j < numInstances; j++){
        float y_coord = int(positions[j]) % NUM_GRID;
        float x_coord = (positions[j] - y_coord) / NUM_GRID;
        glm::vec3 pos = {x_coord, 0.0f, y_coord};
        this->instanceMatrices[j] = glm::translate(glm::mat4(1.0f), TILE_SIZE * pos) * glm::rotate(angles[j], glm::vec3(0.0f, 1.0f, 0.0f));       
    }
    std::cout << "creating instance\n";
    // calling this function leads to a stack smashing error in Release mode
    // tried on ubuntu with cmake gcc
    this->CreateInstanceBuffer(3);
}
StaticModel::~StaticModel(){
    delete[] this->instanceMatrices;
}
int StaticModel::GetNumInstances(){
    return this->numInstances;
}

void StaticModel::CreateInstanceBuffer(GLuint index){
    GLuint buffer = 0;
    glBindVertexArray(this->vertexArrayID);
    glCreateBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, this->numInstances * sizeof(glm::mat4), &this->instanceMatrices[0], GL_STATIC_DRAW);
    for(int j = 0; j < 4; j++){
        glEnableVertexAttribArray(index + j);
        glVertexAttribPointer(index + j, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(j * sizeof(glm::vec4)));
        glVertexAttribDivisor(index + j, 1);
    }
    std::cout << "about to bind NULL\n";
    glBindVertexArray(0);
    std::cout << "unbound vertex array\n";
}