#include "header.h"
StaticModel::StaticModel(const uint& numQuads, float *vertices, float *normals, float *colors, uint numInstances, float* positions, float* angles):
numInstances(numInstances), Model(numQuads, vertices, normals, colors){
    this->instanceMatrices = new glm::mat4[numInstances];
    for(uint j = 0; j < numInstances; j++){
        float y_coord = int(positions[j]) % NUM_GRID;
        float x_coord = (positions[j] - y_coord) / NUM_GRID;
        glm::vec3 pos = {x_coord, 0.0f, y_coord};
        this->instanceMatrices[j] = glm::translate(glm::mat4(1.0f), TILE_SIZE * pos) * glm::rotate(angles[j], glm::vec3(0.0f, 1.0f, 0.0f));       
    }
    this->CreateInstanceBuffer(3);
}
StaticModel::~StaticModel(){
    delete this->instanceMatrices;
}
uint StaticModel::GetNumInstances(){
    return this->numInstances;
}

void StaticModel::CreateInstanceBuffer(uint index){
    uint buffer = 0;
    glBindVertexArray(this->vertexArrayID);
    glCreateBuffers(index, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, this->numInstances * sizeof(glm::mat4), &this->instanceMatrices[0], GL_STATIC_DRAW);
    for(uint j = 0; j < 4; j++){
        glEnableVertexAttribArray(index + j);
        glVertexAttribPointer(index + j, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(j * sizeof(glm::vec4)));
        glVertexAttribDivisor(index + j, 1);
    }
    glBindVertexArray(NULL);
}