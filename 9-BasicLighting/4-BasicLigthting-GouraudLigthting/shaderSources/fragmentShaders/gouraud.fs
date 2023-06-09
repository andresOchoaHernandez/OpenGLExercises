#version 330

in vec3 resultingLigth;

uniform vec3 cubeColor;

void main(){
    gl_FragColor = vec4(resultingLigth*cubeColor,1.0);
}