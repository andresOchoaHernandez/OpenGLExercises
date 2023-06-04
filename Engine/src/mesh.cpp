#include <mesh.hpp>

Mesh::Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,std::vector<Texture> textures,glm::vec3 position):
meshVertices{vertices},
meshIndices{indices},
meshTextures{textures},
positionInWorld{position}
{configureVertexData();}

Mesh::Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,glm::vec3 position):
meshVertices{vertices},
meshIndices{indices},
positionInWorld{position}
{configureVertexData();}

Mesh::Mesh(std::vector<Vertex> vertices,std::vector<Texture> textures,glm::vec3 position):
meshVertices{vertices},
meshTextures{textures},
positionInWorld{position}
{configureVertexData();}

Mesh::Mesh(std::vector<Vertex> vertices,glm::vec3 position):
meshVertices{vertices},
positionInWorld{position}
{configureVertexData();}

void Mesh::configureVertexData()
{
    /* VERTEX ARRAY OBJECT */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* VERTEX BUFFER OBJECT */
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    /* BIND VERTEX DATA */
    glBufferData(GL_ARRAY_BUFFER,meshVertices.size()*sizeof(Vertex),&meshVertices[0], GL_STATIC_DRAW);

    /* ELEMENT BUFFER OBJECT */
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    /* BIND INDICES DATA */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices.size()*sizeof(unsigned int),&meshIndices[0], GL_STATIC_DRAW);

    /* VERTEX COORDS ATTRIBUTE*/
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glEnableVertexAttribArray(0);

    /* VERTEX NORMALS ATTRIBUTE*/
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
    glEnableVertexAttribArray(1);

    /* VERTEX COLORS ATTRIBUTE*/
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,color));
    glEnableVertexAttribArray(2);

    /* TEXTURE COORDINATES */
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,texcoords));
    glEnableVertexAttribArray(3);

    /* SET TO DEFAULTS */
    glBindVertexArray(0);
}

glm::mat4 Mesh::getModelToWorldTransformationMatrix()
{
    return glm::translate(glm::mat4(1.0f),positionInWorld);
}

void Mesh::move(glm::mat4 transformation)
{
    positionInWorld = glm::vec3(transformation*glm::vec4(positionInWorld,1.0f));
}

glm::vec3 Mesh::getPositionInWorld()
{
    return positionInWorld;
}

void Mesh::updatePositionInWorld(glm::vec3 newPos)
{
    positionInWorld = newPos;
}

void Mesh::draw()
{
    glBindVertexArray(VAO);

    if (meshIndices.size() == 0)
        glDrawArrays(GL_TRIANGLES,0,meshVertices.size());
    else
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshIndices.size()), GL_UNSIGNED_INT, 0);

    /* SET TO DEFAULTS */
    glBindVertexArray(0);
}

Cube::Cube(glm::vec3 color,glm::vec3 position):
Mesh(
    {
        {{-1.0f,-1.0f,-1.0f}, {0.0f,-1.0f,0.0f}, color},     
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,-1.0f,0.0f}, color},         
        {{ 1.0f,-1.0f,-1.0f}, {0.0f,-1.0f,0.0f}, color},       
        {{-1.0f,-1.0f, 1.0f}, {0.0f,-1.0f,0.0f}, color},          
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,-1.0f,0.0f}, color},              
        {{-1.0f,-1.0f,-1.0f}, {0.0f,-1.0f,0.0f}, color},

    
        {{ 1.0f,1.0f,-1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{ 1.0f,1.0f, 1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{-1.0f,1.0f,-1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{-1.0f,1.0f,-1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{ 1.0f,1.0f, 1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{-1.0f,1.0f, 1.0f}, {0.0f, 1.0f,0.0f}, color},

        
        {{-1.0f,-1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},                  
        {{ 1.0f,-1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},
        {{ 1.0f, 1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},         
        {{ 1.0f, 1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},         
        {{-1.0f, 1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},              
        {{-1.0f,-1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},

        
        {{-1.0f, 1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},                
        {{ 1.0f, 1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},         
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},         
        {{-1.0f,-1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},              
        {{-1.0f, 1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},

    
        {{ 1.0f, 1.0f,-1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f,-1.0f,-1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f, 1.0f, 1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f, 1.0f, 1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f,-1.0f,-1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f,-1.0f, 1.0f}, {1.0f,0.0f,0.0f}, color},

        
        {{-1.0f, 1.0f, 1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f,-1.0f, 1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f, 1.0f,-1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f,-1.0f, 1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f,-1.0f,-1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f, 1.0f,-1.0f}, {-1.0f,0.0f,0.0f}, color}
    },
    position
)
{}