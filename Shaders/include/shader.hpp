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

        unsigned int getProgramId() const;
        
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
};