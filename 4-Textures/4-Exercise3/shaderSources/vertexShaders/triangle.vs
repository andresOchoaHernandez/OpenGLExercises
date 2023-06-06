#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec2 vTexCoords;

out vec3 color;
out vec2 texCoords;

void main(){
    gl_Position = vec4(vPos,1.0);
    color = vColor;
    texCoords = vTexCoords;
}