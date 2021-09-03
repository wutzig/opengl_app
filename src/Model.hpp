#pragma once
#include "../include/glm/glm.hpp"
#include <vector>
#include "Entity.hpp"
typedef unsigned int GLuint;

class Model{
public:
    Model(const int& numQuads, float *vertices, float *normals, float *colors);
    Model();
    bool operator==(const Model& other);
    float *vertices, *normals, *colors;
    inline int GetVertexArrayID() { return this->vertexArrayID; }
    inline int GetNumQuads() { return this->numQuads; }
    void Bind();
    void Unbind();
private:
    int numQuads;
protected:
    GLuint vertexArrayID;
};

class StaticModel : public Model{
public:
    StaticModel(const int& numQuads, float *vertices, float *normals, float *colors, int numInstances, float* positions, float* angles);
    ~StaticModel();
    int GetNumInstances();
private:    
    int numInstances;
    void CreateInstanceBuffer(GLuint index);
    glm::mat4 *instanceMatrices;
};
class DynamicModel : public Model{
public:
    DynamicModel(const int numQuads, float *vertices, float *normals, float *colors);
    std::vector<Entity> entities;
};