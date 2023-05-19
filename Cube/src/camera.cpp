#include <camera.hpp>

Camera::Camera(float pitch,float yaw,glm::vec3 position,glm::vec3 upVersor,float speed):
cameraPitch{pitch},
cameraYaw{cameraYaw},
cameraPosition{position},
cameraUpVersor{upVersor},
cameraSpeed{speed}
{}

glm::mat4 Camera::getWorldToViewTransformationMatrix()
{
    glm::vec3 direction = glm::vec3(
        cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),
        sin(glm::radians(cameraPitch)),
        sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))
    );
    return glm::lookAt(cameraPosition, cameraPosition + glm::normalize(direction), cameraUpVersor);
}

void Camera::setPitch(float pitch)
{
    cameraPitch = pitch;
}
void Camera::setYaw(float yaw)
{
    cameraYaw = yaw;
}
void Camera::setPosition(glm::vec3 position)
{
    cameraPosition = position; 
}
void Camera::setUpVersor(glm::vec3 upVersor)
{
    cameraUpVersor = upVersor;
}

void Camera::updatePitch(float deltaPitch)
{
    cameraPitch += deltaPitch;
}
void Camera::updateYaw(float deltaYaw)
{
    cameraYaw += deltaYaw;
}
void Camera::updatePosition(glm::vec3 deltaPosition)
{
    cameraPosition += cameraSpeed * deltaPosition;
}