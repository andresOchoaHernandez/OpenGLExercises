#pragma once

#include <iostream>

class Shader
{
    unsigned int _programId;

    public:
        Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath);
        void use();
};