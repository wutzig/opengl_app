#version 440
in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 passColor;
out vec4 shadowCoord;

uniform mat4 projectionViewMatrix;
uniform mat4 depthProjectionViewMatrix;

void main() {
    gl_Position = projectionViewMatrix * vec4(vertexPosition, 1.0);
    shadowCoord = depthProjectionViewMatrix * vec4(vertexPosition, 1.0);
    passColor = vertexColor;
};