#include <camera.hpp>

Camera::Camera(float pitch,float yaw,glm::vec3 position,glm::vec3 direction,glm::vec3 upVersor,float speed,float sensitivity):
cameraPitch{pitch},
cameraYaw{cameraYaw},
cameraPosition{position},
direction{direction},
cameraUpVersor{upVersor},
cameraSpeed{speed},
cameraSensitivity{sensitivity}
{}

glm::mat4 Camera::getWorldToViewTransformationMatrix()
{
    return glm::lookAt(cameraPosition, cameraPosition + direction, cameraUpVersor);
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
void Camera::setCameraSpeed(float speed)
{
    cameraSpeed = speed;
}

void Camera::updatePitchAndYaw(float xoffset, float yoffset)
{
    xoffset *= cameraSensitivity;
    yoffset *= cameraSensitivity;

    cameraYaw   += xoffset;
    cameraPitch += yoffset;

    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;

    direction = glm::vec3(glm::normalize(glm::vec3(cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),sin(glm::radians(cameraPitch)),sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)))));
}

void Camera::moveForward()
{
    cameraPosition += cameraSpeed * direction;
}
void Camera::moveBack()
{
    cameraPosition -= cameraSpeed * direction;
}
void Camera::moveRigth()
{
    cameraPosition += glm::normalize(glm::cross(direction, cameraUpVersor)) * cameraSpeed;
}
void Camera::moveLeft()
{
    cameraPosition -= glm::normalize(glm::cross(direction, cameraUpVersor)) * cameraSpeed;
}