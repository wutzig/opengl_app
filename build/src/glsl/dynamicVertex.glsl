#version 440
in vec3 vertexPosition;
in vec3 vertexColor;
in vec3 vertexNormal;

out vec3 passColor;
out vec3 passNormal;
out vec4 shadowCoord;

uniform mat4 projectionViewMatrix;
uniform mat4 transformationMatrix;
uniform mat4 depthProjectionViewMatrix;

void main() {
    vec4 worldPosition = transformationMatrix * vec4(vertexPosition, 1.0);
    gl_Position = projectionViewMatrix * worldPosition;
    shadowCoord = depthProjectionViewMatrix * worldPosition;
    passColor = vertexColor;
    passNormal = (transformationMatrix * vec4(vertexNormal, 0.0)).xyz;
};