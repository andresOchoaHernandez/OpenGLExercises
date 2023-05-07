#version 330 core

out vec4 FragColor;

in vec3 myColor;

uniform vec4 myColorUnif;

void main()
{
    FragColor = vec4(myColor,1.0) + myColorUnif;
};