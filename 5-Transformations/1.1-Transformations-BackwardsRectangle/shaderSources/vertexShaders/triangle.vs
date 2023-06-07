#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoords;

out vec3 color;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

void main(){
    gl_Position = clip * view * model * vec4(vPos,1.0);
    color = vColor;
    texCoords = vTexCoords;
}