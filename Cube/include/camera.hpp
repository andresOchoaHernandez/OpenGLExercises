#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    float cameraPitch;
    float cameraYaw;

    glm::vec3 cameraPosition;
    glm::vec3 cameraUpVersor;
    glm::vec3 direction;

    float cameraSpeed;

    float cameraSensitivity;

    public:
        Camera(float pitch,float yaw,glm::vec3 position,glm::vec3 direction,glm::vec3 upVersor,float speed,float sensitivity);

        glm::mat4 getWorldToViewTransformationMatrix();

        void setPitch(float pitch);
        void setYaw(float yaw);
        void setPosition(glm::vec3 position);
        void setUpVersor(glm::vec3 upVersor);
        void setCameraSpeed(float speed);

        void updatePitchAndYaw(float xoffset, float yoffset);

        void moveForward();
        void moveBack();
        void moveRigth();
        void moveLeft();
};