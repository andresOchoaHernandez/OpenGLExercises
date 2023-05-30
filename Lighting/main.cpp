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

Camera camera = Camera();

bool firstMouse = true;
float lastX = 400, lastY = 300;

/* HANDLING KEYBOARD INPUT */
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

    /* DISABLE CURSOR AND CAPTURE ITS MOVEMENT */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, 800, 600);

    /* REGISTERING CALLBACKS */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback); 
    /*============================================= SETTING UP THE RECTANGLE =============================================*/
    float vertices[] = {
        /* positions */       /* normals */      /* colors */   
         0.5f,  0.5f, 0.0f,   0.0f, 0.0f,-1.0f,  1.0f, 0.0f, 0.0f,      
         0.5f, -0.5f, 0.0f,   0.0f, 0.0f,-1.0f,  1.0f, 0.0f, 0.0f,      
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,-1.0f,  1.0f, 0.0f, 0.0f,    
        -0.5f,  0.5f, 0.0f,   0.0f, 0.0f,-1.0f,  1.0f, 0.0f, 0.0f,

         0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,    
         0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,    
        -0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,  
        -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,     
    };
    unsigned int indices[] = {
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,9*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    /* VERTEX NORMALS */
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,9*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    /* VERTEX COLORS */
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,9*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    /*===================================================================================================================*/
    Shader shaders("../shaderSources/vertexShaders/triangle.vs","../shaderSources/fragmentShaders/textures.fs");
    shaders.use();

    /*================================================== LIGTH ==========================================================*/
    glm::vec3 ligthColor = glm::vec3(1.0f,1.0f,1.0f);

    unsigned int lightColorLoc = glGetUniformLocation(shaders.getProgramId(),"ligthColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(ligthColor));

    float ambientStrength = 0.8f;
    shaders.setFloat("ambientStrength",ambientStrength);

    glm::vec3 ligthPosition = glm::vec3(0.0f,0.0f,3.0f);
    unsigned int lightPosLoc = glGetUniformLocation(shaders.getProgramId(),"ligthPosition");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(ligthPosition));

    /*===================================================================================================================*/

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);
        
        /* RENDERING COMMANDS */
        glEnable(GL_DEPTH_TEST); 
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /*========================================= CAMERA & TRANSFORMATIONS ================================================*/
        glm::mat4 modelToWorld = glm::rotate(glm::mat4(1.0f),glm::radians(0.0f),glm::vec3(1.0f,0.0f,0.0f)) + glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.f));

        unsigned int modelLoc = glGetUniformLocation(shaders.getProgramId(),"model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelToWorld));

        glm::mat4 worldToView = camera.getWorldToViewTransformationMatrix();

        unsigned int worldLoc = glGetUniformLocation(shaders.getProgramId(),"view");
        glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(worldToView));

        glm::mat4 viewToClip   = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

        unsigned int clipLoc = glGetUniformLocation(shaders.getProgramId(),"clip");
        glUniformMatrix4fv(clipLoc, 1, GL_FALSE, glm::value_ptr(viewToClip));
        /*===================================================================================================================*/
        
        unsigned int cameraPositionLoc = glGetUniformLocation(shaders.getProgramId(),"camPosition");
        glUniform3fv(cameraPositionLoc, 1, glm::value_ptr(camera.getCamPosition()));

        shaders.use();
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