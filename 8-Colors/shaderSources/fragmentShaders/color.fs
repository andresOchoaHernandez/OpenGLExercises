#version 330

in vec3 normal;

uniform vec3 cubeColor;
uniform vec3 ligthColor;

void main(){
    gl_FragColor = vec4(ligthColor*cubeColor,1.0);
}