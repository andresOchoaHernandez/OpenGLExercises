#version 330

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;

out vec4 color;
out vec3 fragPosition;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

void main(){
    gl_Position = clip*view*model*vec4(vPos,1.0);
    color = vec4(vColor,1.0);
    fragPosition = vec3(model*vec4(vPos,1.0));
    normal = mat3(transpose(inverse(model))) * vNormal;
}