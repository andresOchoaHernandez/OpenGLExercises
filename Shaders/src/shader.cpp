#include <shader.hpp>

#include <glad/glad.h>
#include <fstream>
#include <sstream>

#define SHADER_LOG_LEN 512

Shader::Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath)
{
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    vertexShaderFile.exceptions(std::ifstream::failbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit);

    std::stringstream buffer;

    std::string vertexShaderSource;
    std::string fragmentShaderSource;

    try
    {
        vertexShaderFile.open(vertexShaderPath);
        buffer << vertexShaderFile.rdbuf();
        vertexShaderSource = buffer.str();

        vertexShaderFile.close();

        buffer.str(std::string());

        fragmentShaderFile.open(fragmentShaderPath);
        buffer << fragmentShaderFile.rdbuf();
        fragmentShaderSource = buffer.str();

        fragmentShaderFile.close();
    }
    catch(const std::ifstream::failure& error)
    {
        std::cout << "SHADER::ERROR::FILE(S) NOT FOUND" << std::endl;
        return;
    }

    const char* vertexShaderCode   = vertexShaderSource.c_str();
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    int success;
    char infoLog[SHADER_LOG_LEN];

    unsigned int vertex;
    glShaderSource(vertex, 1,&vertexShaderCode,nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertex, SHADER_LOG_LEN, nullptr, infoLog);
        std::cout << "SHADER::ERROR::VERTEX_SHADER::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    };

    unsigned int fragment;
    glShaderSource(fragment, 1, &fragmentShaderCode,nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, SHADER_LOG_LEN, nullptr, infoLog);
        std::cout << "SHADER::ERROR::FRAGMENT_SHADER::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
        return;
    };

    _programId = glCreateProgram();
    glAttachShader(_programId, vertex);
    glAttachShader(_programId, fragment);
    glLinkProgram(_programId);

    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(_programId, SHADER_LOG_LEN, nullptr, infoLog);
        std::cout << "SHADER::ERROR::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(_programId);
}