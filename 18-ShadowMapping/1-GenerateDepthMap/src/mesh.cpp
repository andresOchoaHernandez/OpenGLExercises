#include <mesh.hpp>

#include <stb_image.hpp>

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, std::vector<Texture> tex):
vertices{verts},
indices{inds},
textures{tex}
{
    /* VERTEX ARRAY OBJECT */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
  
    /* VERTEX BUFFER OBJECT */
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); 

    /* ELEMENT BUFFER OBJECT */
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    /* VERTEX ATTRIBUTES */
    
    /* VERTEX POSITIONS */
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    /* VERTEX NORMALS */
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
    
    /* VERTEX TEXTURE COORDINATES */
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,textureCoordinates));

    /* VERTEX TANGENT */
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    
    /* VERTEX BITANGENT */
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    
    /* VERTEX BONE IDS */
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4,GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	/* VERTEX BONE WEIGTHS */
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
    
    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if( name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        else if(name == "texture_normal")
        {
            number = std::to_string(normalNr++);
        }
        else if(name == "texture_height")
        {
            number = std::to_string(heightNr++);
        }

        glUniform1i(glGetUniformLocation(shader.getProgramId(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

/* WALL */
std::vector<glm::vec3> calculateTangentandBitanget(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,glm::vec2 q1,glm::vec2 q2,glm::vec2 q3)
{
    glm::vec3 e1 = p2 - p1;    
    glm::vec3 e2 = p3 - p1;

    glm::vec2 t1 = q2 - q1;    
    glm::vec2 t2 = q3 - q1;

    float f = 1.0f / (t1.x * t2.y - t2.x * t1.y);

    glm::vec3 tangent = glm::vec3(
        f * (t2.y * e1.x - t1.y * e2.x),
        f * (t2.y * e1.y - t1.y * e2.y),
        f * (t2.y * e1.z - t1.y * e2.z)
    );

    glm::vec3 biTangent = glm::vec3(
        f * (-t2.x * e1.x + t1.x * e2.x),
        f * (-t2.x * e1.y + t1.x * e2.y),
        f * (-t2.x * e1.z + t1.x * e2.z)
    );

    return {tangent,biTangent};
}

std::vector<Vertex> getPlaneData()
{
    /* 3D POINTS */
    glm::vec3 p1(-1.0f,  1.0f, 0.0f);
    glm::vec3 p2(-1.0f, -1.0f, 0.0f);
    glm::vec3 p3( 1.0f, -1.0f, 0.0f);
    glm::vec3 p4( 1.0f,  1.0f, 0.0f);
    
    /* UV COORDINATES */
    glm::vec2 q1(0.0f, 1.0f);
    glm::vec2 q2(0.0f, 0.0f);
    glm::vec2 q3(1.0f, 0.0f);  
    glm::vec2 q4(1.0f, 1.0f);
    
    /* NORMAL VECTOR */
    glm::vec3 n(0.0f, 0.0f, 1.0f);

    std::vector<glm::vec3> t1 = calculateTangentandBitanget(p1,p2,p3,q1,q2,q3);
    std::vector<glm::vec3> t2 = calculateTangentandBitanget(p1,p3,p4,q1,q3,q4);

    return {
        {p1,n,q1,t1[0],t1[1]},
        {p2,n,q2,t1[0],t1[1]},
        {p3,n,q3,t1[0],t1[1]},

        {p1,n,q1,t2[0],t2[1]},
        {p3,n,q3,t2[0],t2[1]},
        {p4,n,q4,t2[0],t2[1]}
    };
}

std::vector<Texture> getWallTextures()
{
    unsigned int diffuseTexture = loadTexture("../resources/textures/wall/brickwall.jpg");
    unsigned int normalTexture  = loadTexture("../resources/textures/wall/brickwall_normal.jpg");

    return 
    {
        {diffuseTexture,std::string("texture_diffuse"),std::string("../resources/textures/wall/brickwall.jpg")},
        {normalTexture, std::string("texture_normal"),std::string("../resources/textures/wall/brickwall_normal.jpg")},
    };
}

Wall::Wall():
Mesh
(
    getPlaneData(),
    {},
    getWallTextures()
)
{}

void Wall::Draw(Shader& shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if( name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        else if(name == "texture_normal")
        {
            number = std::to_string(normalNr++);
        }
        else if(name == "texture_height")
        {
            number = std::to_string(heightNr++);
        }

        glUniform1i(glGetUniformLocation(shader.getProgramId(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

Floor::Floor():
Mesh(
    getPlaneData(),
    {},
    {
        {loadTexture("../resources/textures/floor/wood.png"),std::string("texture_diffuse"),std::string("../resources/textures/floor/wood.png")},
        {loadTexture("../resources/textures/floor/wood_normal_map.png"),std::string("texture_normal"),std::string("../resources/textures/floor/wood_normal_map.png")}
    }
)
{}

void Floor::Draw(Shader& shader)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = textures[i].type;
        if( name == "texture_diffuse")
        {
            number = std::to_string(diffuseNr++);
        }
        else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++);
        }
        else if(name == "texture_normal")
        {
            number = std::to_string(normalNr++);
        }
        else if(name == "texture_height")
        {
            number = std::to_string(heightNr++);
        }

        glUniform1i(glGetUniformLocation(shader.getProgramId(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

Cube::Cube():
Mesh(
    {
        {glm::vec3( 0.5f,  0.5f, 0.0f)},
        {glm::vec3( 0.5f, -0.5f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.0f)},
        {glm::vec3(-0.5f,  0.5f, 0.0f)},

        {glm::vec3( 0.5f,  0.5f, 1.0f)},
        {glm::vec3( 0.5f, -0.5f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, 1.0f)}
    },
    {
        0, 1, 3, 
        1, 2, 3,   

        4, 5, 7,
        5, 6, 7,

        4, 5, 0,
        5, 1, 0,

        3, 2, 7,
        2, 6, 7,

        1, 5, 2,
        5, 6, 2,

        4, 0, 7,
        0, 3, 7
    },
    {}
)
{}

Quad::Quad():
Mesh
(
    {
        {glm::vec3(-1.0f,  1.0f, 0.0f),{},glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-1.0f, -1.0f, 0.0f),{},glm::vec2(0.0f, 0.0f)},
        {glm::vec3( 1.0f,  1.0f, 0.0f),{},glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f, -1.0f, 0.0f),{},glm::vec2(1.0f, 0.0f)}
    },
    {},
    {}
)
{}

void Quad::Draw(Shader& shader)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP,0,vertices.size());
    
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}