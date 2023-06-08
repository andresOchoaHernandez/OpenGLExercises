#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

void main(){
    normal = normal;
    gl_Position = clip * view * model * vec4(vPos,1.0);
}