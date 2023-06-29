#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include <shader.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

struct Texture
{
    unsigned int id;
    std::string type;
};

class Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO, VBO, EBO;

    public:
        Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> tex);

        void Draw(Shader& shader);
};