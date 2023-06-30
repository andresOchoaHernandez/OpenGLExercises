#include <shader.hpp>

/* ============================ UTILITY FUNCTIONS ============================*/
std::string readShaderSourceFile(const std::string& shaderPath)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit);

    std::stringstream buffer;
    std::string shaderSource = "";

    try
    {
        shaderFile.open(shaderPath);
        buffer << shaderFile.rdbuf();
        shaderSource = buffer.str();
        
        shaderFile.close();
        buffer.str(std::string());
    }
    catch(const std::ifstream::failure& error)
    {
        std::cout << "SHADER::ERROR::FILE [" << shaderPath <<"] NOT FOUND" << std::endl;
    }

    return shaderSource;
}

bool compileShader(unsigned int& shaderId, unsigned int shaderType ,const char* shaderSource)
{
    glShaderSource(shaderId,1,&shaderSource,nullptr);
    glCompileShader(shaderId);

    int success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        int lenght;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &lenght);
        
        char* log = new char[lenght];
        glGetShaderInfoLog(shaderId,lenght,nullptr,log);
        std::cout << "SHADER::ERROR::" << (shaderType == GL_FRAGMENT_SHADER ? "FRAGMENT_SHADER":"VERTEX_SHADER") << "::COMPILATION_FAILED" << std::endl << log << std::endl;
        delete[] log;

        return false;
    };

    return true;
}

bool linkShaders(unsigned int& vertexShaderId,unsigned int& fragmentShaderId,unsigned int& programId)
{
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    int success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if(!success)
    {
        int length;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
        
        char* log = new char[length];
        glGetProgramInfoLog(programId, length, nullptr,log);
        
        std::cout << "SHADER::ERROR::PROGRAM::LINKING_FAILED" << std::endl << log << std::endl;
        delete[] log;
        
        return false;
    }

    return true;
}
/* ========================================================================== */

Shader::Shader(const std::string& vertexShaderPath,const std::string& fragmentShaderPath)
{
    std::string vertexShaderSource   = readShaderSourceFile(vertexShaderPath);
    std::string fragmentShaderSource = readShaderSourceFile(fragmentShaderPath);

    if(vertexShaderSource.empty() || fragmentShaderSource.empty()){return;}

    /* SHADER SOURCES */
    const char* vertexShaderCode   = vertexShaderSource.c_str();
    const char* fragmentShaderCode = fragmentShaderSource.c_str();

    /* VERTEX SHADER CREATION & COMPILATION */
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    bool vertexShaderCompilationStatus = compileShader(vertex,GL_VERTEX_SHADER,vertexShaderCode); 

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    bool fragmentShaderCompilationStatus = compileShader(fragment,GL_FRAGMENT_SHADER,fragmentShaderCode);

    if(!vertexShaderCompilationStatus || !fragmentShaderCompilationStatus){return;}

    /* PROGRAM CREATION & LINKING */
    _programId = glCreateProgram();
    bool linkShadersStatus = linkShaders(vertex,fragment,_programId);

    if(!linkShadersStatus){return;}

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(_programId);
}

unsigned int Shader::getProgramId() const
{
    return _programId;
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(_programId,name.c_str()),static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(_programId,name.c_str()),value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(_programId,name.c_str()),value);
}

void Shader::setMatrix4f(const std::string& name, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(_programId,name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVector3f(const std::string& name, glm::vec3 vector) const
{
    glUniform3fv(glGetUniformLocation(_programId,name.c_str()),1,glm::value_ptr(vector));
}