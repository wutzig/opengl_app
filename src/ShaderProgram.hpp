#pragma once
typedef unsigned int GLuint;
class ShaderProgram{
public:
    ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath = nullptr);
    ~ShaderProgram();
    GLuint GetProgramID();
private:
    GLuint LoadShader(const char* file_path, GLuint type);
    void DeleteProgram();
    int programID, vertexShaderID, fragmentShaderID, geometryShaderID;
};