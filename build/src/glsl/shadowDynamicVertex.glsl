#version 330 core

in vec3 vertexPosition;
uniform mat4 depthProjectionViewMatrix;
uniform mat4 transformationMatrix;

void main(){
    gl_Position = depthProjectionViewMatrix * transformationMatrix * vec4(vertexPosition, 1.0);
}