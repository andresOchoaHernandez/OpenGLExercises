#version 330 core

in vec2 texCoords;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture(depthMap, texCoords).r;
    gl_FragColor = vec4(vec3(depthValue), 1.0);
}