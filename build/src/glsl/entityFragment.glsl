#version 440
precision highp float;

uniform sampler2D shadowMap;
uniform vec3 lightPos;

in vec4 shadowCoord;
in vec3 passColor;
in vec3 passNormal;

out vec4 fragmentColor;
vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);
float bias = 0.0005;
void main() {
    float visibility = 1.0;
    vec3 projCoords = 0.5 * shadowCoord.xyz + 0.5;
    for (int i=0;i<4;i++){
        if ( texture( shadowMap, projCoords.xy + poissonDisk[i]/1800.0 ).z  <  projCoords.z-bias ){
            visibility-=0.1;
        }
    }
    visibility = max(0.4, min( visibility, dot(passNormal, normalize(lightPos))));
    vec3 finalColor = visibility * passColor;

    fragmentColor = vec4(finalColor, 1.0);
}