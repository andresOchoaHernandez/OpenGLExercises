#version 330

in vec3 color;
in vec2 texCoords;

uniform sampler2D vTexture1;
uniform sampler2D vTexture2;

uniform float alpha;

void main(){
    gl_FragColor = mix(texture(vTexture1,texCoords),texture(vTexture2,texCoords),alpha);
}