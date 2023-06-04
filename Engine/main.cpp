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
#include <ligth.hpp>
#include <mesh.hpp>

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGTH = 600;

Camera camera = Camera();

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
bool firstMouse = true;
float lastX = 400, lastY = 300;

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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH,"Engine",nullptr,nullptr);
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

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH);

    /* REGISTERING CALLBACKS */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* SCENE OBJECTS */
    Cube cube(glm::vec3(1.0f, 0.5f, 0.2f),glm::vec3(0.0f,0.0f,0.0f));
    Ligth ligth(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(10.0f,10.0f,10.0f));

    /* SHADERS */
    Shader renderShaders("../shaderSources/vertexShaders/render.vs","../shaderSources/fragmentShaders/render.fs");
    Shader ligthCubeShaders("../shaderSources/vertexShaders/ligthCube.vs","../shaderSources/fragmentShaders/ligthCube.fs");

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);

        /* RENDERING COMMANDS */
        glEnable(GL_DEPTH_TEST); 
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* ==============================  SCENE TRANSFORMATIONS  ======================================== */
        /* CUBE  TO WORLD */
        glm::mat4 cubeToWorld = cube.getModelToWorldTransformationMatrix();

        /* LIGTH CUBE TO WORLD */
        glm::mat4 ligthToWorld = ligth.getModelToWorldTransformationMatrix();

        /* WORLD TO VIEW  & VIEW TO CLIP*/
        glm::mat4 worldToView = camera.getWorldToViewTransformationMatrix();
        glm::mat4 viewToClip  = glm::perspective(glm::radians(camera.getZoom()), static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGTH), camera.getNearVal(), camera.getFarVal());
        /* =============================================================================================== */

        /*========================================= LIGTH CUBE DRAWING ====================================*/
        ligthCubeShaders.use();

        ligthCubeShaders.setMatrix4fv("model",ligthToWorld);
        ligthCubeShaders.setMatrix4fv("view",worldToView);
        ligthCubeShaders.setMatrix4fv("clip",viewToClip);

        ligth.draw();
        /*================================================================================================*/

        /*============================== RENDERING THE WHOLE SCENE =======================================*/
        renderShaders.use();

        renderShaders.setMatrix4fv("model",cubeToWorld);
        renderShaders.setMatrix4fv("view",worldToView);
        renderShaders.setMatrix4fv("clip",viewToClip);

        ligth.setColorUniform(renderShaders.getProgramId(),"ligthColor");
        ligth.setPositionUniform(renderShaders.getProgramId(),"ligthPosition");

        cube.draw();
        /*===============================================================================================*/

        /* EVENTS AND BUFFER SWAP */
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}