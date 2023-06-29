#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include <camera.hpp>
#include <mesh.hpp>

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGTH = 600;

Camera camera;

/* HANDLING INPUT */
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBack();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft();
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRigth();
}

/* HANDLING MOUSE INPUT */
bool firstMouse = true;
float lastX = SCREEN_WIDTH/2, lastY = SCREEN_HEIGTH/2;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
  
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera.updatePitchAndYaw(xoffset,yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.updateZoom(yoffset);
}

/* THIS CALLBACK FUNCTION WILL BE TRIGGERED WHEN THE USER RESIZES THE WINDOW. IT WILL SET THE NEW GL VIEWPORT */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

int main(int argc, char *argv[])
{
    glfwInit();

    /* SPECIFYING OPENGL VERSION */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    /* WE WANT ONLY THE CORE FUNCTIONALITIES */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* CREATING A WINDOW */
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Model loading",nullptr,nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    /* THE CONTEXT OF THE WINDOW WILL BE THE CONTEXT OF THIS THREAD */
    glfwMakeContextCurrent(window);

    /* INITIALIZE GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH);

    glEnable(GL_DEPTH_TEST);

    /* REGISTERING CALLBACKS */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    /*============================================= SETTING UP THE VERTICES =============================================*/
    float vertices[] = {

        /* vertex pos */      /* normals */    /* texture coordinates */
        -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f, 
         1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,       
         1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f, /* DOWNER FACE */
        -1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,        
         1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,            
        -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f,
                
         1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  
         1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, /* UPPER FACE */
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 
         1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 
        -1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,    
        
        -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,                  
         1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f, 
         1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f, /* BACK FACE */       
         1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f,         
        -1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,              
        -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f, 

        -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,               
         1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, /* FRONT FACE */    
         1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,       
        -1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,          
        -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        
         1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, /* LATERAL DX FACE */  
         1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, /* LATERAL SX FACE */
        -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    };
    /* VERTEX ARRAY OBJECT */
    unsigned int CUBEVAO;
    glGenVertexArrays(1, &CUBEVAO);
    glBindVertexArray(CUBEVAO);

    /* VERTEX BUFFER OBJECT */
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    /* PROPERTIES */

    /* VERTEX COORDS */
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    /* VERTEX NORMALS */
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    /* TEXTURE COORDINATES */
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    /* LIGTH CUBE */
    unsigned int LIGTHVAO;
    glGenVertexArrays(1, &LIGTHVAO);
    glBindVertexArray(LIGTHVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*===================================================================================================================*/

    /*=============================================== TEXTURE ===========================================================*/
    
    /* DIFFUSE */
    
    /* CREATE TEXTURE OBJECT */
    unsigned int diffuseMap;
    glGenTextures(1,&diffuseMap);
    glBindTexture(GL_TEXTURE_2D,diffuseMap);

    /* SET PROPERTIES OF THE TEXTURE */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    /* READ THE IMAGE  AND BIND IT WITH THE TEXTURE OBJECT*/
    int width, height, nrChannels;
    unsigned char * data = stbi_load("../resources/tex/container2.png",&width,&height,&nrChannels,0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: COULD NOT READ TEXTURE" << std::endl;
    }
    stbi_image_free(data);

    /* SPECULAR */

    unsigned int specularMap;
    glGenTextures(1,&specularMap);
    glBindTexture(GL_TEXTURE_2D,specularMap);

    /* SET PROPERTIES OF THE TEXTURE */
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    /* READ THE IMAGE  AND BIND IT WITH THE TEXTURE OBJECT*/
    data = stbi_load("../resources/tex/container2_specular.png",&width,&height,&nrChannels,0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR: COULD NOT READ TEXTURE" << std::endl;
    }
    stbi_image_free(data);
    /*===================================================================================================================*/
    
    Shader renderShaders("../shaderSources/vertexShaders/scene.vs","../shaderSources/fragmentShaders/phongligthting.fs");
    Shader ligthShaders("../shaderSources/vertexShaders/ligthCube.vs","../shaderSources/fragmentShaders/ligthCube.fs");

    renderShaders.use();

    /* MATERIAL PROPERTIES */
    renderShaders.setInt("material.diffuse",0);
    renderShaders.setInt("material.specular",1);
    renderShaders.setFloat("material.shininess",32.0f);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  4.0f, -15.0f),
        glm::vec3(-5.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -5.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 9.5f,  4.0f, -2.5f),
        glm::vec3( 8.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  7.0f, -1.5f)
    };

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);

        /* RENDERING COMMANDS */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderShaders.use();

        /* DIRECTIONAL LIGTH */
        renderShaders.setVector3f("dirLigthDir",glm::vec3(-0.2f,-1.0f,-0.3f));
        renderShaders.setVector3f("directionalLigth.ambient", glm::vec3(0.2f,0.2f,0.2f));
        renderShaders.setVector3f("directionalLigth.diffuse", glm::vec3(0.25f,0.25f,0.25f));  
        renderShaders.setVector3f("directionalLigth.specular",glm::vec3(1.0f, 1.0f, 1.0f));

        /* POINT LIGTH */
        glm::vec3 pointLigthPosition = glm::vec3(3.0f, 3.0f,-7.0f);
        glm::vec3 pointLigthColor = glm::vec3(0.0f,1.0f,0.0f);

        glm::vec3 pointLigthDiffuse = pointLigthColor * glm::vec3(0.2f);
        glm::vec3 pointLigthAmbient = pointLigthDiffuse * glm::vec3(0.5f);

        renderShaders.setVector3f("pointLigthPos", pointLigthPosition);
        renderShaders.setFloat("pointLigth.constant",1.0f);
        renderShaders.setFloat("pointLigth.linear",0.09f);
        renderShaders.setFloat("pointLigth.quadratic",0.032f);
        renderShaders.setVector3f("pointLigth.ambient", pointLigthAmbient);
        renderShaders.setVector3f("pointLigth.diffuse", pointLigthDiffuse);  
        renderShaders.setVector3f("pointLigth.specular",glm::vec3(1.0f, 1.0f, 1.0f));
    
        /* SPOT LIGTH */
        renderShaders.setVector3f("flashLigthPos",camera.getCamPosition());
        renderShaders.setVector3f("flashLigthDir",camera.getCamDirection());

        renderShaders.setFloat("spotLigth.innerCutOff",glm::cos(glm::radians(12.5f)));
        renderShaders.setFloat("spotLigth.outerCutOff",glm::cos(glm::radians(17.5f)));
        renderShaders.setFloat("spotLigth.constant",1.0f);
        renderShaders.setFloat("spotLigth.linear",0.09f);
        renderShaders.setFloat("spotLigth.quadratic",0.032f);

        glm::vec3 spotLigthColor = glm::vec3(1.0f,0.0f,0.0f);

        glm::vec3 spotLigthDiffuse = spotLigthColor * glm::vec3(0.2f);
        glm::vec3 spotLigthAmbient = spotLigthDiffuse * glm::vec3(0.5f);

        renderShaders.setVector3f("spotLigth.ambient", spotLigthAmbient);
        renderShaders.setVector3f("spotLigth.diffuse", spotLigthDiffuse);  
        renderShaders.setVector3f("spotLigth.specular",glm::vec3(1.0f, 1.0f, 1.0f));
        
        /* ========================================================= TRANSFORMATIONS ======================================== */

        glm::mat4 worldToView = camera.getWorldToViewTransformationMatrix();
        glm::mat4 viewToClip   = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGTH), camera.getNearVal(), camera.getFarVal());

        for(int i = 0; i < 10 ; i++)
        {
            glm::mat4 cubeToWorld = glm::rotate(glm::translate(glm::mat4(1.0f),cubePositions[i]),glm::radians(20.0f*i),glm::vec3(1.0f,0.3f,0.5f));

            renderShaders.setMatrix4f("model",cubeToWorld);
            renderShaders.setMatrix4f("view",worldToView);
            renderShaders.setMatrix4f("clip",viewToClip);

            /* ACTIVATE THE DIFFUSE MAP */
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            
            /* ACTIVATE THE SPECULAR MAP */
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularMap);
            
            /* DRAW THE CUBE */
            glBindVertexArray(CUBEVAO);
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        /*====================================================================================================================*/
        
        ligthShaders.use();
        ligthShaders.setVector3f("ligthColor",pointLigthColor);

        glm::mat4 ligthCubeToWorld = glm::scale(glm::translate(glm::mat4(1.0f),pointLigthPosition),glm::vec3(0.8f));

        ligthShaders.setMatrix4f("model",ligthCubeToWorld);
        ligthShaders.setMatrix4f("view",worldToView);
        ligthShaders.setMatrix4f("clip",viewToClip);
        
        /* DRAW THE LIGTHCUBE */
        glBindVertexArray(LIGTHVAO);
        glDrawArrays(GL_TRIANGLES,0,36);

        /* EVENTS AND BUFFER SWAP */
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &CUBEVAO);
    glDeleteVertexArrays(1, &LIGTHVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}