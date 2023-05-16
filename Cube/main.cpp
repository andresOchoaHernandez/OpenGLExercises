#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>

/* HANDLING INPUT */
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWindow",nullptr,nullptr);
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

    glViewport(0, 0, 800, 600);

    /* REGISTERING CALLBACK */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*============================================= SETTING UP THE RECTANGLE =============================================*/

    float vertices[] = {
           
         1.0f, 0.0f, -1.0f,
         1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f,  1.0f,
        -1.0f, 0.0f, -1.0f,
           
         1.0f, 1.0f, -1.0f,
         1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f,  1.0f,
        -1.0f, 1.0f, -1.0f,     
    };
    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0,

        7, 2, 3,
        6, 2, 7,
        
        0, 1, 4,
        1, 5, 4,
        
        3, 0, 4,
        7, 3, 4,
        
        7, 4, 5,
        7, 5, 6,
        
        6, 5, 1,
        1, 2, 6,
    };

    /* VERTEX ARRAY OBJECT */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* VERTEX BUFFER OBJECT */
    unsigned int VBO;
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    /* ELEMENT BUFFER OBJECT */
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* PROPERTIES */

    /* VERTEX COORDS */
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    /*============================================ TRANSFORMATIONS ======================================================*/

    /* object to world transformation */
    glm::mat4 worldTransformation = glm::mat4(1.0f);
    worldTransformation = glm::translate(worldTransformation,glm::vec3(1.0f,1.0f,1.0f)); 
    
    /* world to view transformation */    
    glm::mat4 viewTransformation = glm::mat4(1.0f);
    viewTransformation = glm::rotate(viewTransformation,glm::radians(0.0f),glm::vec3(0.0f,-10.0f,0.0f));

    /* view to clip transformation */
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    /*===================================================================================================================*/
    Shader shaders("../shaderSources/vertexShaders/cube.vs","../shaderSources/fragmentShaders/orange.fs");
    shaders.use();

    unsigned int worldLocation = glGetUniformLocation(shaders.getProgramId(),"world");
    glUniformMatrix4fv(worldLocation, 1, GL_FALSE, glm::value_ptr(worldTransformation));

    unsigned int viewLocation = glGetUniformLocation(shaders.getProgramId(),"view");
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewTransformation));

    unsigned int projectionLocation = glGetUniformLocation(shaders.getProgramId(),"projection");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);
        
        /* RENDERING COMMANDS */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,36, GL_UNSIGNED_INT, 0);

        /* EVENTS AND BUFFER SWAP */
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}