#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    unsigned int _programId;

    public:
        Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
        void use();

        unsigned int getProgramId();
};