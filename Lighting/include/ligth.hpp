#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Ligth
{
    glm::vec3 ligthColor;
    glm::vec3 ligthPosition;

    public:
        Ligth(glm::vec3 color,glm::vec3 position);

        glm::vec3 getColor();
        glm::vec3 getPosition();

        void setColorUniform(unsigned int programId,const std::string& name);
        void setPositionUniform(unsigned int programId,const std::string& name);

        void setPosition(glm::vec3 newPosition);
};