#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

class Cube
{
    std::vector<Vertex> vertices;
    glm::vec3 color;
    glm::vec3 position;

    public:
        Cube(glm::vec3 color, glm::vec3 worldPosition);
        glm::mat4 getModelToWorldTransformationMatrix();
        void draw();

        void move(glm::mat4 transformation);
        glm::vec3 getPosition();
        void updatePosition(glm::vec3 newPos);

    private:
        unsigned int VAO;
        unsigned int VBO;
};