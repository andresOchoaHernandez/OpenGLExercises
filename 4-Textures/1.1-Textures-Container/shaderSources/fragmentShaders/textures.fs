#version 330

in vec3 color;
in vec2 texCoords;

uniform sampler2D vTexture1;

void main(){
    gl_FragColor = texture(vTexture1,texCoords) * vec4(color,1.0);
}