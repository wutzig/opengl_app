#include "../include/GL/glew.h"
#include "Renderer.hpp"
#include "ShaderProgram.hpp"
#include "App.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_set>
#include <stdlib.h>
#include "../include/GL/glew.h"
#include "../include/GLFW/glfw3.h"
#include "../include/glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/transform.hpp" 

Renderer* renderer;
bool rightMouseButtonPressed = false;
double mousePressX = 0.0, mousePressY = 0.0, mousePosX = 0.0, mousePosY = 0.0;
void resizeViewport(GLFWwindow* window, int width, int height){
    renderer->UpdatePerspective(float(width), float(height));
    glViewport(0, 0, width, height);
}
void mouseMove(GLFWwindow* window, double mouseX, double mouseY){
    mousePosX = mouseX;
    mousePosY = mouseY;
    if(!rightMouseButtonPressed){
        renderer->UpdateMousePos(mouseX, mouseY);
        renderer->ShootRay();
    }
    else{
        float deltaMouseX = float(mousePosX - mousePressX);
        float deltaMouseY = float(mousePosY - mousePressY);
        renderer->UpdateView(deltaMouseX, deltaMouseY);
        mousePressX = mouseX;
        mousePressY = mouseY;
    }
}
void mouseButtonPress(GLFWwindow* window, int button, int action, int mods){
    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        if(action == GLFW_PRESS){
            rightMouseButtonPressed = true;
            mousePressX = mousePosX;
            mousePressY = mousePosY;
        }
        else{
            rightMouseButtonPressed = false;
        }
    }
}
int main(){
    GLFWwindow* window;

    std::cout << glfwGetVersionString() << '\n';
    int glfwSuccess = glfwInit();
    if(!glfwSuccess){
        std::cout << "glfw not initialized\n";
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(INIT_WIDTH, INIT_HEIGHT, "GameWindow", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    GLenum err = glewInit();
    std::cout<<"glew error: "<<err<<std::endl;
    if(err != GLEW_OK){
        std::cout << "glew not initialized with " << glewGetErrorString(err);
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glfwSetFramebufferSizeCallback(window, resizeViewport);
    glfwSetCursorPosCallback(window, mouseMove);
    glfwSetMouseButtonCallback(window, mouseButtonPress);
    std::cout<<glGetString(GL_VERSION)<<std::endl;
    double now_time = glfwGetTime();
    double old_time = now_time;
    double time_since_frame = 0.0;
    uint frames = 0;

    renderer = new Renderer(float(INIT_WIDTH), float(INIT_HEIGHT));
    std::cout<<"creating terrain program...";
    ShaderProgram terrainProgram("../src/glsl/terrainVertex.glsl", "../src/glsl/terrainFragment.glsl");
    
    ShaderProgram staticProgram("../src/glsl/staticVertex.glsl", "../src/glsl/entityFragment.glsl");
    ShaderProgram dynamicProgram("../src/glsl/dynamicVertex.glsl", "../src/glsl/entityFragment.glsl");
    ShaderProgram shadowStaticProgram("../src/glsl/shadowStaticVertex.glsl", "../src/glsl/shadowFragment.glsl");
    ShaderProgram shadowDynamicProgram("../src/glsl/shadowDynamicVertex.glsl", "../src/glsl/shadowFragment.glsl");
    ShaderProgram guiProgram("../src/glsl/guiVertex.glsl", "../src/glsl/guiFragment.glsl");

    renderer->GetUniformLocations(terrainProgram.GetProgramID(), 
                                  staticProgram.GetProgramID(), dynamicProgram.GetProgramID(),
                                  shadowStaticProgram.GetProgramID(), shadowDynamicProgram.GetProgramID());

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
     
        now_time = glfwGetTime();
        renderer->DrawScene(
            terrainProgram.GetProgramID(), 
            staticProgram.GetProgramID(), dynamicProgram.GetProgramID(), 
            shadowStaticProgram.GetProgramID(), shadowDynamicProgram.GetProgramID(),
            guiProgram.GetProgramID(), now_time - old_time);
        
        frames++;
        time_since_frame += now_time - old_time;
        if(time_since_frame >= 1.0){
            std::cout<<"FPS: "<< double(frames) / time_since_frame << "\r";
            time_since_frame = 0.0;
            frames = 0;
        }
        old_time = now_time;
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete renderer;
}