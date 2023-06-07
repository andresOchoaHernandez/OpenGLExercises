#version 330

in vec2 texCoords;

uniform sampler2D floorTexture;

void main(){
    gl_FragColor = texture(floorTexture,texCoords);
}