#version 330 core

in vec2 texCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main()
{    
    //gl_FragColor = texture(texture_diffuse1, texCoords);
    gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}