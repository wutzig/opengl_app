#version 330 core

in vec3 vertexPosition;
in mat4 instanceMatrix;
uniform mat4 depthProjectionViewMatrix;

void main(){
    gl_Position = depthProjectionViewMatrix * instanceMatrix * vec4(vertexPosition, 1.0);
}