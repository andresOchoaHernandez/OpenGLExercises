#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

void main(){
    gl_Position = clip*view*model*vec4(vPos,1.0);
    color = vec4(vColor,1.0);
}