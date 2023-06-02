#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <cube.hpp>

class Ligth
{
    glm::vec3 ligthColor;
    glm::vec3 ligthPosition;

    Cube ligthCube;

    public:
        Ligth(glm::vec3 color,glm::vec3 position);

        void setColorUniform(unsigned int programId,const std::string& name);
        void setPositionUniform(unsigned int programId,const std::string& name);

        void setPosition(glm::vec3 newPosition);

        glm::mat4 getModelToWorldTransformationMatrix();
        void draw();
};