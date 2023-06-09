#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 normal;
out vec3 fragposition;
out vec3 ligthPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 ligthPos;

void main(){
    normal = mat3(transpose(inverse(view * model)))*vNormal;
    fragposition = vec3(view * model * vec4(vPos,1.0));
    ligthPosition = vec3(view * vec4(ligthPos, 1.0));
    gl_Position = clip * view * model * vec4(vPos,1.0);
}