#include "ShaderProgram.hpp"
#include <iostream>
#include <fstream>
#include "../include/GL/glew.h"
ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath){
    this->programID = glCreateProgram();
    this->vertexShaderID = LoadShader(vertexShaderPath, GL_VERTEX_SHADER);
    std::cout<<"vertex shader loaded!"<<std::endl;
    glBindAttribLocation(this->programID, 0, "vertexPosition");
    glBindAttribLocation(this->programID, 1, "vertexColor");
    glBindAttribLocation(this->programID, 2, "vertexNormal");
    glBindAttribLocation(this->programID, 3, "instanceMatrix");
    glAttachShader(this->programID, this->vertexShaderID);
    std::cout<<"vertex shader attached!"<<std::endl;
    this->fragmentShaderID = LoadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
    std::cout<<"fragment shader loaded!"<<std::endl;
    glAttachShader(this->programID, this->fragmentShaderID);
    std::cout<<"fragment shader attached!"<<std::endl;
    if(geometryShaderPath != NULL){
        this->geometryShaderID = LoadShader(geometryShaderPath, GL_GEOMETRY_SHADER);
        glAttachShader(this->programID, this->geometryShaderID);
    }
    std::cout<<"Linking program...\n"<<std::endl;
    glLinkProgram(this->programID);
    int isLinked = 0;
    glGetProgramiv(this->programID, GL_LINK_STATUS, &isLinked);

    if(isLinked == GL_FALSE){
        int maxLength = 0;
        char* err = new char[maxLength + 1];
        glGetProgramiv(this->programID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetProgramInfoLog(this->programID, maxLength, &maxLength, err);
        std::cout<<err;
        glDeleteProgram(this->programID);
        return;
    }
}
ShaderProgram::~ShaderProgram(){
    this->DeleteProgram();
}

uint ShaderProgram::GetProgramID(){
    return this->programID;
}
void ShaderProgram::DeleteProgram(){
    glDetachShader(this->programID, this->vertexShaderID);
    glDeleteShader(this->vertexShaderID);

    glDetachShader(this->programID, this->geometryShaderID);
    glDeleteShader(this->geometryShaderID);

    glDetachShader(this->programID, this->geometryShaderID);
    glDeleteShader(this->geometryShaderID);

    glDeleteProgram(this->programID);
    glUseProgram(0);
}
GLuint ShaderProgram::LoadShader(const char* file_path, GLuint type){
    std::string shaderString;
    std::ifstream sourceFile( file_path );
    std::cout << "source " << file_path << '\n';
    if( sourceFile ){
        shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
    }
    const char* source = shaderString.c_str();
    
    uint shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    int isCompiled = 0;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){
        int maxLength = 0;
        char* err = new char[maxLength + 1];
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, err);
        std::cout<<"Shader Compile error: "<<err<<std::endl;
        glDeleteShader(shaderID);
    }
    return shaderID;
}