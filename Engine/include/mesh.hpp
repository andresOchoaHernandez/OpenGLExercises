#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include <shader.hpp>

struct Texture
{
    unsigned int id;
    std::string type;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texcoords;
};

class Mesh
{
    protected:
        std::vector<Vertex> meshVertices;
        std::vector<unsigned int> meshIndices;
        std::vector<Texture> meshTextures;

        /* WORLD PROPERTIES */
        glm::vec3 positionInWorld;

        unsigned int VAO,VBO,EBO;
        void configureVertexData();

    public:
        Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures,glm::vec3 position);
        Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,glm::vec3 position);
        Mesh(std::vector<Vertex> vertices,std::vector<Texture> textures,glm::vec3 position);
        Mesh(std::vector<Vertex> vertices,glm::vec3 position);

        /* TRANSFORMATIONS IN WORLD */
        glm::mat4 getModelToWorldTransformationMatrix();

        void draw();
};

class Cube : public Mesh
{
    public:
        Cube(glm::vec3 color,glm::vec3 position);
};