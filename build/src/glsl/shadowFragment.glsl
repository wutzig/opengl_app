#version 330 core
out float fragmentDepth;

void main(){
    fragmentDepth = gl_FragCoord.z;
}