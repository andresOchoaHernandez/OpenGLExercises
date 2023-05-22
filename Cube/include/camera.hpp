#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    public:
        Camera();

        glm::mat4 getWorldToViewTransformationMatrix();

        void updatePitchAndYaw(float xoffset, float yoffset);

        void moveForward();
        void moveBack();
        void moveRigth();
        void moveLeft();
    
    private:
        void updateDirection();
};