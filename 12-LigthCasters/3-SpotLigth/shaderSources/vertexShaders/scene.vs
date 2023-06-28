#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 normal;
out vec3 fragposition;
out vec3 ligthPosition;
out vec3 ligthDir;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 ligthPos;
uniform vec3 ligthDirection;

void main(){
    normal = mat3(transpose(inverse(view * model)))*vNormal;
    fragposition = vec3(view * model * vec4(vPos,1.0));
    ligthPosition = vec3(view * vec4(ligthPos, 1.0));
    /* 
        ALTHOUGH INVERSING A MATRIX IS EXPENSIVE, IT'S NECESSARY IN ORDER TO AVOID THE SPOTLIGTH
        BEING "POINTED" AT 0.0.0. FURTHERMORE LIGTHDIR IT'S A VERSOR SO, LIKE THE NORMALS, WE 
        HAVE TO AVOID "STRETCHES".
     */
    ligthDir = mat3(transpose(inverse(view)))*ligthDirection;
    texCoords = vTexCoords;
    gl_Position = clip * view * model * vec4(vPos,1.0);
}