#version 330

uniform vec3 ligthColor;

void main(){
    gl_FragColor = vec4(ligthColor,1.0);
}