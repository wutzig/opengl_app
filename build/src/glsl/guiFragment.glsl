#version 330 core
uniform sampler2D shadowMap;

in vec2 UV;
out vec4 fragColor;

void main(){
    float val = texture( shadowMap, UV).r;
    vec3 color = vec3(val);
    fragColor = vec4(color, 1.0);
}