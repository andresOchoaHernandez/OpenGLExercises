#include <cube.hpp>

Cube::Cube(glm::vec3 color,glm::vec3 worldPosition):
color{color},
position{worldPosition},
vertices{
    {
        /* coordinates */    /* normals */
        
        /* downer face */
        {{-1.0f,-1.0f,-1.0f}, {0.0f,-1.0f,0.0f}, color},     
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,-1.0f,0.0f}, color},         
        {{ 1.0f,-1.0f,-1.0f}, {0.0f,-1.0f,0.0f}, color},       
        {{-1.0f,-1.0f, 1.0f}, {0.0f,-1.0f,0.0f}, color},          
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,-1.0f,0.0f}, color},              
        {{-1.0f,-1.0f,-1.0f}, {0.0f,-1.0f,0.0f}, color},

        /* upper face */
        {{ 1.0f,1.0f,-1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{ 1.0f,1.0f, 1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{-1.0f,1.0f,-1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{-1.0f,1.0f,-1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{ 1.0f,1.0f, 1.0f}, {0.0f, 1.0f,0.0f}, color},
        {{-1.0f,1.0f, 1.0f}, {0.0f, 1.0f,0.0f}, color},

        /* back face */
        {{-1.0f,-1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},                  
        {{ 1.0f,-1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},
        {{ 1.0f, 1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},         
        {{ 1.0f, 1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},         
        {{-1.0f, 1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},              
        {{-1.0f,-1.0f,-1.0f}, {0.0f,0.0f,-1.0f}, color},

        /* front face */
        {{-1.0f, 1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},                
        {{ 1.0f, 1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},         
        {{ 1.0f,-1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},         
        {{-1.0f,-1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},              
        {{-1.0f, 1.0f, 1.0f}, {0.0f,0.0f,1.0f}, color},

        /* lateral rigth face */
        {{ 1.0f, 1.0f,-1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f,-1.0f,-1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f, 1.0f, 1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f, 1.0f, 1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f,-1.0f,-1.0f}, {1.0f,0.0f,0.0f}, color},
        {{ 1.0f,-1.0f, 1.0f}, {1.0f,0.0f,0.0f}, color},

        /* lateral left face */
        {{-1.0f, 1.0f, 1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f,-1.0f, 1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f, 1.0f,-1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f,-1.0f, 1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f,-1.0f,-1.0f}, {-1.0f,0.0f,0.0f}, color},
        {{-1.0f, 1.0f,-1.0f}, {-1.0f,0.0f,0.0f}, color}
    }
}
{    
    /* VERTEX ARRAY OBJECT */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* VERTEX BUFFER OBJECT */
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    /* BIND VERTEX DATA */
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0], GL_STATIC_DRAW);

    /* VERTEX COORDS ATTRIBUTE*/
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glEnableVertexAttribArray(0);

    /* VERTEX NORMALS ATTRIBUTE*/
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,normal));
    glEnableVertexAttribArray(1);

    /* VERTEX COLORS ATTRIBUTE*/
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,color));
    glEnableVertexAttribArray(2);
}

glm::mat4 Cube::getModelToWorldTransformationMatrix()
{
    return glm::translate(glm::mat4(1.0f),position);
}

void Cube::draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,vertices.size());
}

void Cube::move(glm::mat4 transformation)
{
    position = glm::vec3(transformation*glm::vec4(position,1.0f));
}

glm::vec3 Cube::getPosition()
{
    return position;
}

void Cube::updatePosition(glm::vec3 newPos)
{
    position = newPos;
}