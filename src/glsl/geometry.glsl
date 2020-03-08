#version 440

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

const float TILE_SIZE = 1.0 - 0.05;

void createVertex(vec3 direction){
    gl_Position = projectionMatrix * viewMatrix * (gl_in[0].gl_Position + (TILE_SIZE * vec4(direction, 0.0)));
    EmitVertex();
}

void main(){
    createVertex(vec3(0.0, 0.0, 0.0));
    createVertex(vec3(1.0, 0.0, 0.0));
    createVertex(vec3(0.0, 0.0, 1.0));
    createVertex(vec3(1.0, 0.0, 1.0));

    EndPrimitive();
};