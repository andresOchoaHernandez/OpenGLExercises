#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    ourColor = aPos;
    gl_Position = vec4(aPos,1.0);
}

/*
    The triangle's vertex coordinates are:
    
    LEFT  => < -0.5f , -0.5f , 0.0f >
    RIGTH => <  0.5f , -0.5f , 0.0f >
    UP    => <  0.0f ,  0.5f , 0.0f >

    RGB values go from 0 to 1, so the left
    vertex will be colored (0,0,0) (black),
    the rigth one will have (0.5,0.0,0.0) (half red),
    and the upper one will have (0.0,0.5,0.0) (half green)
*/