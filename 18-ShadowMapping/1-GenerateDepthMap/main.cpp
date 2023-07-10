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
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Shadow Mapping",nullptr,nullptr);
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

    /* SHADOW TEXTURE GENERATION */
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader depthShader("../shaderSources/vertexShaders/depthShader.vs","../shaderSources/fragmentShaders/depthShader.fs");
    Shader debugDepth("../shaderSources/vertexShaders/debugDepth.vs","../shaderSources/fragmentShaders/debugDepth.fs");
    Quad quad;

    debugDepth.use();
    debugDepth.setInt("depthMap",0);

    /* MODELS */
    //Shader normalMappingShader("../shaderSources/vertexShaders/lightingShader.vs","../shaderSources/fragmentShaders/lightingShader.fs");
    Model backpack("../resources/backpack/backpack.obj");
    Floor floor;

    /* DIRECTIONAL LIGTH POSITION & DIRECTION */
    glm::vec3 directionalLigthPosition  = glm::vec3(-2.0f, 4.0f, -1.0f);
    //glm::vec3 directionalLigthDirection = glm::vec3(0.0f,1.0f,-1.0f);

    /* RENDER LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* INPUTS */
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* ============== FIRST PASS: RENDER SCENE FROM LIGTH PERSPECTIVE ============== */
        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(directionalLigthPosition,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)); 
        glm::mat4 lightSpaceMatrix = lightProjection * lightView; 

        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));

        depthShader.use();

        depthShader.setMatrix4f("lightSpaceMatrix",lightSpaceMatrix);
        depthShader.setMatrix4f("model",model);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        backpack.Draw(depthShader);
        floor.Draw(depthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /* DEBUG DEPTH */
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGTH);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        debugDepth.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,depthMap);
        quad.Draw(debugDepth);

        /* ==============       SECOND PASS: RENDER SCENE AS USUAL        ============== */
        /*
        glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 view = camera.getWorldToViewTransformationMatrix();
        glm::mat4 clip = glm::perspective(glm::radians(camera.getZoom()), (float)SCREEN_WIDTH/ (float)SCREEN_HEIGTH,camera.getNearVal(),camera.getFarVal());
        
        normalMappingShader.use();

        normalMappingShader.setMatrix4f("model",model);
        normalMappingShader.setMatrix4f("view",view);
        normalMappingShader.setMatrix4f("clip",clip);
    
        // VIEW POSITION 
        normalMappingShader.setVector3f("viewPos",camera.getCamPosition());

        // DIRECTIONAL LIGTH 
        normalMappingShader.setVector3f("dirLigthDir",directionalLigthDirection);
        normalMappingShader.setVector3f("directionalLigth.ambient", glm::vec3(1.0f));
        normalMappingShader.setVector3f("directionalLigth.diffuse", glm::vec3(1.0f));  
        normalMappingShader.setVector3f("directionalLigth.specular",glm::vec3(1.0f));

        // DRAW BACKPACK 
        backpack.Draw(normalMappingShader);

        // DRAW FLOOR 
        model = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,-10.0f,0.0f));
        model = glm::scale(glm::rotate(model,glm::radians(-90.0f),glm::vec3(1.0f,0.0f,0.0f)),glm::vec3(50.0f));
        normalMappingShader.setMatrix4f("model",model);
        
        floor.Draw(normalMappingShader);
        */

        /* EVENTS AND BUFFER SWAP */
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}