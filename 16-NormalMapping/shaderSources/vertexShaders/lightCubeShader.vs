#version 330

layout (location = 0) in vec3  vPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

void main()
{
    gl_Position = clip * view * model * vec4(vPos,1.0);
}