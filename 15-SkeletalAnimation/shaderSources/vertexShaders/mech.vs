#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in vec4 vBoneIds;
layout (location = 6) in vec4 vBoneWeigths;

out vec3 normal;
out vec3 fragposition;
out vec3 ligthPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform mat4 gBones[70];

void main()
{
    vec3 ligthPos = vec3(0.0,0.0,6.0);

    /* ANIMATION */
    mat4 BoneTransform = gBones[ vBoneIds[0] ] * vBoneWeigths[0];
	BoneTransform     += gBones[ vBoneIds[1] ] * vBoneWeigths[1];
    BoneTransform     += gBones[ vBoneIds[2] ] * vBoneWeigths[2];
    BoneTransform     += gBones[ vBoneIds[3] ] * vBoneWeigths[3];
    vec4 tPos = BoneTransform * vec4(vPos, 1.0);
    gl_Position = clip * view * model * tPos;

    normal = mat3(transpose(inverse(view * model)))*vNormal;
    fragposition = vec3(view * model * vec4(vPos,1.0));
    ligthPosition = vec3(view * vec4(ligthPos, 1.0));
}