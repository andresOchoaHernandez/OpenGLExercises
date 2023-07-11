#version 330

layout (location = 0) in vec3  vPos;
layout (location = 1) in vec3  vNormal;
layout (location = 2) in vec2  vTexCoords;
layout (location = 3) in vec3  vTangent;
layout (location = 4) in vec3  vBitangent;
layout (location = 5) in ivec4 vBoneIds;
layout (location = 6) in vec4  vWeights;

out vec3 fragPosition;
out vec3 normal;
out vec2 texCoords;

out LigthSpace
{
    vec4 fragPositionLigthSpace;
}ligthSpace;

/* UNIFORMS */
uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;
uniform mat4 ligthSpaceMatrix;

void main(){

    fragPosition = vec3(model * vec4(vPos,1.0));
    normal       = transpose(inverse(mat3(model))) * vNormal;
    texCoords    = vTexCoords;

    /* FRAGMENT POSITION IN LIGTHSPACE */
    ligthSpace.fragPositionLigthSpace = ligthSpaceMatrix * vec4(fragPosition,1.0);

    /* CALCULATING VERTEX POSITION */
    gl_Position = clip * view * model * vec4(vPos,1.0);
}