#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>

#include <shader.hpp>
#include <camera.hpp>
#include <mesh.hpp>
#include <model.hpp>

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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Skeletal animation",nullptr,nullptr);
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
    
    stbi_set_flip_vertically_on_load(true);

    Shader minotaurShader("../shaderSources/vertexShaders/minotaur.vs", "../shaderSources/fragmentShaders/minotaur.fs");
    Model minotaur("../resources/minotaur/Minotaur@Jump.FBX");

    /* DEBUG */

    std::vector<Mesh> meshes = minotaur.getMeshes();

    float maxX = -1.0f;
    float maxY = -1.0f;
    float maxZ = -1.0f;

    for(int i = 0; i < meshes.size(); i++)
    {
        std::vector<Vertex> v = meshes[i].getVertices();

        for(int j = 0; j < v.size(); j++)
        {
            if(v[j].position.x > maxX) maxX = v[j].position.x;
            if(v[j].position.y > maxY) maxY = v[j].position.y;
            if(v[j].position.z > maxZ) maxZ = v[j].position.z; 
        }
    }

    std::cout << maxX << " " << maxY << " " << maxZ << std::endl;

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);

        /* RENDERING COMMANDS */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        minotaurShader.use();

        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-20.0f,0.0f)),glm::vec3(0.2f));
        glm::mat4 view = camera.getWorldToViewTransformationMatrix();
        glm::mat4 clip = glm::perspective(glm::radians(camera.getZoom()), (float)SCREEN_WIDTH/ (float)SCREEN_HEIGTH,camera.getNearVal(),camera.getFarVal());

        minotaur.Draw(minotaurShader);

        /* EVENTS AND BUFFER SWAP */
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}