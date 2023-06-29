#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

/* LIGTHING REQ ATTRIBS */
out vec3 normal;
out vec3 fragposition;

out vec3 directionalLigthDirection;
out vec3 pointLigthPosition;

out vec3 flashLigthPosition;
out vec3 flashLigthDirection;

out vec2 texCoords;

/* UNIFORMS */
uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 dirLigthDir;
uniform vec3 pointLigthPos;

uniform vec3 flashLigthPos;
uniform vec3 flashLigthDir;

void main(){
    normal = mat3(transpose(inverse(view * model)))*vNormal;
    fragposition = vec3(view * model * vec4(vPos,1.0));

    directionalLigthDirection = vec3(view * vec4(dirLigthDir,1.0));
    pointLigthPosition = vec3(view * vec4(pointLigthPos,1.0));

    flashLigthPosition  = vec3(view * vec4(flashLigthPos, 1.0));
    flashLigthDirection = mat3(transpose(inverse(view)))*flashLigthDir;

    texCoords = vTexCoords;
    
    gl_Position = clip * view * model * vec4(vPos,1.0);
}