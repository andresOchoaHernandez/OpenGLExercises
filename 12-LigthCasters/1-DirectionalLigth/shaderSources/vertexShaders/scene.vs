#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 normal;
out vec3 fragposition;
out vec3 ligthDirection;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 directionalLigth;

void main(){
    normal = mat3(transpose(inverse(view * model)))*vNormal;
    fragposition = vec3(view * model * vec4(vPos,1.0));
    ligthDirection = vec3(view * vec4(directionalLigth, 1.0));
    texCoords = vTexCoords;
    gl_Position = clip * view * model * vec4(vPos,1.0);
}