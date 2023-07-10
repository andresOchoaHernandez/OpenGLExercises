#version 330

layout (location = 0) in vec3  vPos;
layout (location = 1) in vec3  vNormal;
layout (location = 2) in vec2  vTexCoords;
layout (location = 3) in vec3  vTangent;
layout (location = 4) in vec3  vBitangent;
layout (location = 5) in ivec4 vBoneIds;
layout (location = 6) in vec4  vWeights;

out TangentSpace
{
    vec3 fragmentTangentPosition;
    vec3 viewPosition;
    vec3 directionalLigthDirection;
    vec2 texCoords;

} tangentSpace;

/* UNIFORMS */
uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 viewPos;

uniform vec3 dirLigthDir;

void main(){

    /* TBN MATRIX FROM WORLD TO TANGENT SPACE */
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * vTangent);
    vec3 N = normalize(normalMatrix * vNormal);
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(normalize(T - dot(T, N) * N), B, N));

    /* TRANSFORMING ALL RELEVANT LIGTHING DATA TO TANGENT SPACE */
    tangentSpace.fragmentTangentPosition   = TBN * vec3(model * vec4(vPos,1.0));
    tangentSpace.viewPosition              = TBN * viewPos;
    tangentSpace.directionalLigthDirection = TBN * dirLigthDir;
    tangentSpace.texCoords                 = vTexCoords;

    /* CALCULATING VERTEX POSITION */
    gl_Position = clip * view * model * vec4(vPos,1.0);
}