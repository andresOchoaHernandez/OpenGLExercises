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
        /* first downer rectangle */   
         1.0f, 1.0f, 0.0f,
         1.0f,-1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        /* second upper rectangle */   
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,     
    };
    unsigned int indices[] = {
        0, 1, 3,   /* first triangle  */
        1, 2, 3,   /* second triangle */
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
    

    /*===================================================================================================================*/
    Shader shaders("../shaderSources/vertexShaders/cube.vs","../shaderSources/fragmentShaders/orange.fs");
    shaders.use();

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);
        
        /* RENDERING COMMANDS */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6*6, GL_UNSIGNED_INT, 0);

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