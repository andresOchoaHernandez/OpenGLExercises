#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoords;

out vec3 fragposition;
out vec3 normal;
out vec3 color;
out vec3 ligthposition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 clip;

uniform vec3 ligthPosition;

void main(){
    
    fragposition  = vec3(view*model*vec4(vPosition,1.0));
    normal        = mat3(transpose(inverse(view*model))) *vNormal;
    color         = vColor;
    ligthposition = vec3(view * vec4(ligthPosition,1.0)); 

    gl_Position  = clip*view*model*vec4(vPosition,1.0);
}

/* 
    Normal matrix:

    It prevents the normals of beign subject to translations(that's why we take the 3x3)
    and non uniform scaling (that can make normals non perperdicular to the surface).

    How is it derived?

    http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
*/