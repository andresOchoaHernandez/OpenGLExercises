#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Camera
{
    /* WORLD ATTRIBUTES */
    glm::vec3 worldUpVersor;

    /* CAMERA ATTRIBUTES */
    float camPitch;
    float camYaw;

    glm::vec3 camPosition;
    glm::vec3 camDirection;

    glm::vec3 camUpVersor;
    glm::vec3 camRigthVersor;

    /* MOVEMENT ATTRIBUTES */
    float camSpeed;
    float camSensitivity;
    float camZoom;

    float camNear;
    float camFar;

    public:
        Camera();

        glm::mat4 getWorldToViewTransformationMatrix();

        void updatePitchAndYaw(float xoffset, float yoffset);

        glm::vec3 getCamPosition();

        void moveForward();
        void moveBack();
        void moveRigth();
        void moveLeft();

        void updateZoom(float deltaZoom);
        float getZoom();

        float getNearVal();
        float getFarVal();

        void setCameraSpeed(float newSpeed);
        float getCameraSpeed();

        void setPositionUniform(unsigned int programId,const std::string& name);
    
    private:
        void updateDirection();
};