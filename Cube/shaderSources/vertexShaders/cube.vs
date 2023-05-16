#version 330

layout (location = 0) in vec3 vPos;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection*view*world*vec4(vPos,1.0);
}