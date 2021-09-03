#pragma once
#include <vector>
#include "../include/GL/glew.h"
#include "Camera.hpp"
#include <unordered_set>
#include "App.hpp"

class DynamicModel;
class StaticModel;
class Renderer
{
public:
    Renderer(float width, float height);
    ~Renderer();
    void UpdateMousePos(double posX, double posY);
    void DrawScene(
        GLuint terrainShaderProgramID,
        GLuint staticShaderProgramID,
        GLuint dynamicShaderProgramID,
        GLuint shadowStaticShaderProgramID,
        GLuint shadowDynamicShaderProgramID,
        GLuint guiShaderProgramID,
        double delta_time
    );
    void GetUniformLocations(
        GLuint terrainShaderProgramID,
        GLuint staticShaderProgramID,
        GLuint dynamicShaderProgramID,
        GLuint shadowStaticShaderProgramID,
        GLuint shadowDynamicShaderProgramID
    );
    void UpdatePerspective(float width, float height);
    void UpdateView(float deltaX, float deltaY);
    void ShootRay();
    Camera camera;

    void InitShadowMap();
    static glm::vec3 quatRot(const glm::vec3 &point, const glm::vec3 &axis, double angle, const glm::vec3 &origin);

private:
    uint frameBufferID = 0, 
        terrainColorBufferID = 0, 
        terrainVertexArrayID = 0, 
        mousePosX = 0, 
        mousePosY = 0, 
        guiVertexArrayID = 0,
        FramebufferName = 0,
        depthTextureID = 0;
    
    glm::mat4 projectionViewMatrix,
        projectionMatrix,
        viewMatrix,
        depthProjectionViewMatrix,
        invProjectionMatrix,
        invViewMatrix;
    
    glm::vec3 lightPos;
    uint terrainProjectionViewMatrixLoc,
        terrainDepthProjectionViewMatrixLoc,
        staticProjectionViewMatrixLoc,
        staticDepthProjectionViewMatrixLoc,
        staticLightPositionLoc,
        dynamicProjectionViewMatrixLoc,
        dynamicTransformationMatrixLoc,
        dynamicDepthProjectionViewMatrixLoc,
        dynamicLightPositionLoc,
        shadowStaticDepthProjectionViewMatrixLoc,
        shadowDynamicDepthProjectionViewMatrixLoc,
        shadowDynamicTransformationMatrixLoc;

    int select_index = -1;
    std::vector<float> terrain_vertices, terrain_colors, terrain_normals, gui_vertices;
    std::vector<DynamicModel> dynamicModels;
    std::vector<StaticModel> staticModels;
    // std::map<Model*, std::vector<Entity>> entities;
    std::unordered_set<int> blockedTiles;
    void RenderTerrain();
    static void AddTriplet(std::vector<float> *vertices, float x, float y, float z);
    static std::vector<float> CubeVertices(float vxmin, float vxmax, float vymin, float vymax, float vzmin, float vzmax);
    float width, height;
};