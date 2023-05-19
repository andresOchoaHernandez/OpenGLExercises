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

    float cameraSpeed;

    public:
        Camera(float pitch,float yaw,glm::vec3 position,glm::vec3 upVersor,float speed);

        glm::mat4 getWorldToViewTransformationMatrix();

        void setPitch(float pitch);
        void setYaw(float yaw);
        void setPosition(glm::vec3 position);
        void setUpVersor(glm::vec3 upVersor);

        void updatePitch(float deltaPitch);
        void updateYaw(float deltaYaw);
        void updatePosition(glm::vec3 deltaPosition);
};