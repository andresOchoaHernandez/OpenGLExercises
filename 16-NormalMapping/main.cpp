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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Normal mapping",nullptr,nullptr);
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

    Shader normalMappingShader("../shaderSources/vertexShaders/lightingShader.vs","../shaderSources/fragmentShaders/lightingShader.fs");
    Shader ligthCubeShader("../shaderSources/vertexShaders/lightCubeShader.vs","../shaderSources/fragmentShaders/lightCubeShader.fs");

    Wall wall;
    Cube cube;

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);

        /* RENDERING COMMANDS */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))),glm::vec3(10.0f));
        glm::mat4 view = camera.getWorldToViewTransformationMatrix();
        glm::mat4 clip = glm::perspective(glm::radians(camera.getZoom()), (float)SCREEN_WIDTH/ (float)SCREEN_HEIGTH,camera.getNearVal(),camera.getFarVal());
        
        normalMappingShader.use();

        normalMappingShader.setMatrix4f("model",model);
        normalMappingShader.setMatrix4f("view",view);
        normalMappingShader.setMatrix4f("clip",clip);
    
        /* VIEW POSITION */
        normalMappingShader.setVector3f("viewPos",camera.getCamPosition());

        /* DIRECTIONAL LIGTH */
        normalMappingShader.setVector3f("dirLigthDir",glm::vec3(1.0f,1.0f,1.0f));
        normalMappingShader.setVector3f("directionalLigth.ambient", glm::vec3(0.2f,0.2f,0.2f));
        normalMappingShader.setVector3f("directionalLigth.diffuse", glm::vec3(0.25f,0.25f,0.25f));  
        normalMappingShader.setVector3f("directionalLigth.specular",glm::vec3(1.0f, 1.0f, 1.0f));

        /* POINT LIGTH */
        glm::vec3 pointLigthPosition = glm::vec3(2.0f,1.0f,3.0f);
        glm::vec3 pointLigthColor = glm::vec3(1.0f,1.0f,1.0f);

        glm::vec3 pointLigthDiffuse = pointLigthColor * glm::vec3(0.2f);
        glm::vec3 pointLigthAmbient = pointLigthDiffuse * glm::vec3(0.5f);

        normalMappingShader.setVector3f("pointLigthPos", pointLigthPosition);
        normalMappingShader.setFloat("pointLigth.constant",1.0f);
        normalMappingShader.setFloat("pointLigth.linear",0.1f);
        normalMappingShader.setFloat("pointLigth.quadratic",0.035f);
        normalMappingShader.setVector3f("pointLigth.ambient", pointLigthAmbient);
        normalMappingShader.setVector3f("pointLigth.diffuse", pointLigthDiffuse);  
        normalMappingShader.setVector3f("pointLigth.specular",glm::vec3(1.0f, 1.0f, 1.0f));

        /* SPOT LIGTH POSITION AND DIRECTION */

        normalMappingShader.setVector3f("flashLigthPos",camera.getCamPosition());
        normalMappingShader.setVector3f("flashLigthDir",camera.getCamDirection());

        /* SPOT LIGTH PROPERTIES */
        normalMappingShader.setFloat("spotLigth.innerCutOff",glm::cos(glm::radians(12.5f)));
        normalMappingShader.setFloat("spotLigth.outerCutOff",glm::cos(glm::radians(17.5f)));
        normalMappingShader.setFloat("spotLigth.constant",1.0f);
        normalMappingShader.setFloat("spotLigth.linear",0.09f);
        normalMappingShader.setFloat("spotLigth.quadratic",0.032f);
        glm::vec3 spotLigthColor = glm::vec3(1.0f,1.0f,1.0f);
        glm::vec3 spotLigthDiffuse = spotLigthColor * glm::vec3(0.2f);
        glm::vec3 spotLigthAmbient = spotLigthDiffuse * glm::vec3(0.5f);

        normalMappingShader.setVector3f("spotLigth.ambient", spotLigthAmbient);
        normalMappingShader.setVector3f("spotLigth.diffuse", spotLigthDiffuse);  
        normalMappingShader.setVector3f("spotLigth.specular",glm::vec3(1.0f, 1.0f, 1.0f));

        /* DRAW BRICKWALL */
        wall.Draw(normalMappingShader);

        /* LIGTH CUBE REPRESENTS POINT LIGTH */
        ligthCubeShader.use();
        model = glm::scale(glm::translate(glm::mat4(1.0f),pointLigthPosition),glm::vec3(0.5f));
        ligthCubeShader.setMatrix4f("model",model);
        ligthCubeShader.setMatrix4f("view",view);
        ligthCubeShader.setMatrix4f("clip",clip);
        ligthCubeShader.setVector3f("ligthColor",pointLigthColor);
        cube.Draw(ligthCubeShader);


        /* EVENTS AND BUFFER SWAP */
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}