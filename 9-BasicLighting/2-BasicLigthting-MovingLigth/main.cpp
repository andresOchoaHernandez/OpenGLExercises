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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "BasicLigthting",nullptr,nullptr);
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
    /*============================================= SETTING UP THE RECTANGLE =============================================*/
    float vertices[] = {
        -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,   
         1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,         
         1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,       
        -1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,          
         1.0f,-1.0f, 1.0f,  0.0f,-1.0f, 0.0f,              
        -1.0f,-1.0f,-1.0f,  0.0f,-1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,  0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,    
        -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,                  
         1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,
         1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,         
         1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,         
        -1.0f, 1.0f,-1.0f,  0.0f, 0.0f,-1.0f,              
        -1.0f,-1.0f,-1.0f,  0.0f, 0.0f,-1.0f,    
        -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,               
         1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,         
         1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,         
        -1.0f,-1.0f, 1.0f,  0.0f, 0.0f, 1.0f,              
        -1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,  1.0f, 0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    /* VERTEX NORMALS */
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    unsigned int LIGTHVAO;
    glGenVertexArrays(1, &LIGTHVAO);
    glBindVertexArray(LIGTHVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*===================================================================================================================*/

    Shader renderShaders("../shaderSources/vertexShaders/scene.vs","../shaderSources/fragmentShaders/phongligthting.fs");
    Shader ligthShaders("../shaderSources/vertexShaders/ligthCube.vs","../shaderSources/fragmentShaders/ligthCube.fs");

    glm::vec3 ligthColor    = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 ligthPosition = glm::vec3(3.0f,3.0f,-3.0f);

    glm::vec3 cubeColor  = glm::vec3(1.0f, 0.5f, 0.2f); 

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);

        /* RENDERING COMMANDS */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(CUBEVAO);
        
        /* ROTATING LIGTH */
        ligthPosition = glm::vec3(glm::rotate(glm::mat4(1.0f),0.1f*sin((float)glfwGetTime())*glm::radians(50.0f),glm::vec3(1.0f,1.0f,1.0f)) * glm::vec4(ligthPosition,1.0f));

        renderShaders.use();
        renderShaders.setVector3f("cubeColor",cubeColor);
        renderShaders.setVector3f("ligthColor",ligthColor);
        renderShaders.setVector3f("ligthPosition",ligthPosition);
        renderShaders.setVector3f("viewPosition",camera.getCamPosition());
        /* ========================================================= TRANSFORMATIONS ======================================== */
        glm::mat4 cubeToWorld = glm::mat4(1.0f);
        glm::mat4 worldToView = camera.getWorldToViewTransformationMatrix();
        glm::mat4 viewToClip   = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGTH), camera.getNearVal(), camera.getFarVal());

        renderShaders.setMatrix4f("model",cubeToWorld);
        renderShaders.setMatrix4f("view",worldToView);
        renderShaders.setMatrix4f("clip",viewToClip);
        /*====================================================================================================================*/

        glDrawArrays(GL_TRIANGLES,0,36);

        glBindVertexArray(LIGTHVAO);
        ligthShaders.use();
        ligthShaders.setVector3f("ligthColor",ligthColor);

        glm::mat4 ligthCubeToWorld = glm::translate(glm::mat4(1.0f),ligthPosition);

        ligthShaders.setMatrix4f("model",ligthCubeToWorld);
        ligthShaders.setMatrix4f("view",worldToView);
        ligthShaders.setMatrix4f("clip",viewToClip);
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