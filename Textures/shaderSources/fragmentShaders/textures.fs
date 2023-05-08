#version 330

in vec4 color;
in vec2 texCoords;

uniform sampler2D vTexture1;
uniform sampler2D vTexture2;

void main(){
    gl_FragColor = mix(texture(vTexture1,texCoords)*color,texture(vTexture2,vec2(-texCoords.x,texCoords.y)),0.2);
}