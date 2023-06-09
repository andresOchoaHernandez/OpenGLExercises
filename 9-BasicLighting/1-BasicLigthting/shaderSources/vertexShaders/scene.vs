#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 normal;
out vec3 fragposition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

void main(){
    normal = mat3(transpose(inverse(model)))*vNormal;
    fragposition = vec3(model * vec4(vPos,1.0));
    gl_Position = clip * view * model * vec4(vPos,1.0);
}