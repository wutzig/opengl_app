#version 330 core

in vec3 vertexPosition;
out vec2 UV;

void main(){
    gl_Position =  vec4(vertexPosition, 1.0);
    UV = vertexPosition.xy;
}