#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_set>
#include <stdlib.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp" 

using namespace std;
typedef unsigned int uint;

const uint INIT_WIDTH = 800;
const uint INIT_HEIGHT = 600;
const uint SHADOW_WIDTH = glm::pow(2, 12);
const uint SHADOW_HEIGHT = glm::pow(2, 12);
const uint NUM_GRID = 19;
const uint NUM_TREES = 60;
const float OFFSET = 0.05f;
const float TILE_SIZE = 1.0f;
const float PI = 3.14159;
const float INIT_CAMERA_YAW = 0.0f;
const float INIT_CAMERA_PITCH = -PI / 6.0f;

const float dark_green[12] = {0.4f, 0.5f, 0.2f,0.4f, 0.5f, 0.2f,0.4f, 0.5f, 0.2f,0.4f, 0.5f, 0.2f};
const float light_green[12] = {0.6f, 0.7f, 0.3f, 0.6f, 0.7f, 0.3f, 0.6f, 0.7f, 0.3f, 0.6f, 0.7f, 0.3f};
const float light_blue[12] = {0.4f, 0.4f, 0.8f,0.4f, 0.4f, 0.8f,0.4f, 0.4f, 0.8f,0.4f, 0.4f, 0.8f};

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

class ShaderProgram{
public:
    ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = NULL);
    ~ShaderProgram();
    uint GetProgramID();
private:
    uint LoadShader(const char* path, uint type);
    void DeleteProgram();
    uint programID, vertexShaderID, fragmentShaderID, geometryShaderID;
};
class Camera{
public:
    Camera();
    Camera(float pitch, float yaw, glm::vec3 lookat);
    void Rotate(float deltaYaw, float deltaPitch);
    glm::vec3 position, right, lookat;
private:
    float yaw, pitch;
};

class Entity{
public:
    Entity(glm::vec3 position = glm::vec3(0.0f), float rotationY = 0.0f);
    void SetRotation(float newRotation);
    void SetPosition(glm::vec3 newPosition);
    void SetScale(float newScale);
    float* GetTransformationMatrix();
    void ChangePosition(double delta_time, const std::unordered_set<uint>* blockedTiles);
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

class Model{
public:
    Model(const uint& numQuads, float *vertices, float *normals, float *colors);
    Model();
    bool operator==(const Model& other);
    float *vertices, *normals, *colors;
    inline uint GetVertexArrayID() { return this->vertexArrayID; }
    inline uint GetNumQuads() { return this->numQuads; }
    void Bind();
    void Unbind();
private:
    uint numQuads;
protected:
    uint vertexArrayID;
};

class StaticModel : public Model{
public:
    StaticModel(const uint& numQuads, float *vertices, float *normals, float *colors, uint numInstances, float* positions, float* angles);
    ~StaticModel();
    uint GetNumInstances();
private:    
    uint numInstances;
    void CreateInstanceBuffer(uint index);
    glm::mat4 *instanceMatrices;
};

class DynamicModel : public Model{
public:
    DynamicModel(const uint& numQuads, float *vertices, float *normals, float *colors);
    std::vector<Entity> entities;
};

class Renderer{
    public:
        Renderer(float width, float height);
        ~Renderer();
        void UpdateMousePos(double posX, double posY);
        void DrawScene(uint terrainShaderProgramID, 
                       uint staticShaderProgramID, uint dynamicShaderProgramID,
                       uint shadowStaticShaderProgramID, uint shadowDynamicShaderProgramID,
                       uint guiShaderProgramID, double delta_time);
        void GetUniformLocations(uint terrainShaderProgramID,
                                 uint staticShaderProgramID, uint dynamicShaderProgramID,
                                 uint shadowStaticShaderProgramID, uint shadowDynamicShaderProgramID);
        void UpdatePerspective(float width, float height);
        void UpdateView(float deltaX, float deltaY);
        void ShootRay();
        Camera camera;
        
        void InitShadowMap();
        static glm::vec3 quatRot(const glm::vec3& point, const glm::vec3& axis, double angle, const glm::vec3& origin);
    private:
        uint frameBufferID = 0, terrainColorBufferID = 0, terrainVertexArrayID = 0, mousePosX = 0, mousePosY = 0, guiVertexArrayID = 0,
        FramebufferName = 0, depthTextureID = 0;
        glm::mat4 projectionViewMatrix, projectionMatrix, viewMatrix, depthProjectionViewMatrix,
        invProjectionMatrix, invViewMatrix;
        glm::vec3 lightPos;
        uint terrainProjectionViewMatrixLoc, terrainDepthProjectionViewMatrixLoc, 
        staticProjectionViewMatrixLoc, staticDepthProjectionViewMatrixLoc, staticLightPositionLoc,
        dynamicProjectionViewMatrixLoc, dynamicTransformationMatrixLoc, dynamicDepthProjectionViewMatrixLoc, dynamicLightPositionLoc,
        shadowStaticDepthProjectionViewMatrixLoc, shadowDynamicDepthProjectionViewMatrixLoc,
        shadowDynamicTransformationMatrixLoc;
        int select_index = -1;
        std::vector<float> terrain_vertices, terrain_colors, terrain_normals, gui_vertices;
        std::vector<DynamicModel> dynamicModels;
        std::vector<StaticModel> staticModels;
        // std::map<Model*, std::vector<Entity>> entities;
        std::unordered_set<uint> blockedTiles;
        void RenderTerrain();
        static void AddTriplet(std::vector<float>* vertices, float x, float y, float z);
        static std::vector<float> CubeVertices(float vxmin, float vxmax, float vymin, float vymax, float vzmin, float vzmax);
        float width, height;
};