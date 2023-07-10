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
    glm::vec3 tangent;
    glm::vec3 bitangent;
	unsigned int m_BoneIDs[4];  /* BONES THAT INF. ON THIS VERTEX */
	float m_Weights[4];/* BONES WEIGTHS */
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
    protected:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        unsigned int VAO, VBO, EBO;

    public:
        Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> tex);
        void Draw(Shader& shader);        
};

class Wall : public Mesh
{
    public:
        Wall();
        void Draw(Shader& shader);
};

class Floor : public Mesh
{
    public:
        Floor();
        void Draw(Shader& shader);
};

class Cube : public Mesh
{
    public:
        Cube();
};