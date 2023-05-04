#include <shader.hpp>

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


    /* SHADER SOURCES */
    const char* vertexShaderCode   = vertexShaderSource.c_str();
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    /* VERTEX SHADER CREATION & COMPILATION */
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vertexShaderCode,nullptr);
    glCompileShader(vertex);
    
    /* LOGGIN ERRORS */
    int success;
    int lenght;

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &lenght);
        char* log = new char[lenght];
        glGetShaderInfoLog(vertex,lenght,nullptr,log);
        std::cout << "SHADER::ERROR::VERTEX_SHADER::COMPILATION_FAILED" << std::endl << log << std::endl;
        delete[] log;
        return;
    };

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode,nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderiv(fragment,GL_INFO_LOG_LENGTH, &lenght);
        char* log = new char[lenght];
        glGetShaderInfoLog(fragment,lenght,nullptr,log);
        std::cout << "SHADER::ERROR::FRAGMENT_SHADER::COMPILATION_FAILED" << std::endl << log << std::endl;
        delete[] log;
        return;
    };

    _programId = glCreateProgram();
    glAttachShader(_programId, vertex);
    glAttachShader(_programId, fragment);
    glLinkProgram(_programId);

    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &lenght);
        char* log = new char[lenght];
        glGetProgramInfoLog(_programId, lenght, nullptr,log);
        std::cout << "SHADER::ERROR::PROGRAM::LINKING_FAILED" << std::endl << log << std::endl;
        delete[] log;
        return;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(_programId);
}