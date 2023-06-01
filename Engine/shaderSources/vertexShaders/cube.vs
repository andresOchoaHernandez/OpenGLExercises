#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;

out vec3 normal;
out vec3 color;

uniform mat4 cube;
uniform mat4 view;
uniform mat4 clip;

void main(){
    normal = vNormal;
    color  = vColor;
    gl_Position = clip*view*cube*vec4(vPosition,1.0);
}