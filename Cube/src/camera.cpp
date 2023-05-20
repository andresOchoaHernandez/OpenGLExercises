#include <camera.hpp>

Camera::Camera(float pitch,float yaw,glm::vec3 position,glm::vec3 direction,glm::vec3 upVersor,glm::vec3 worldUpVersor,float speed,float sensitivity):
cameraPitch{pitch},
cameraYaw{cameraYaw},
cameraPosition{position},
direction{direction},
cameraUpVersor{upVersor},
worldUpVersor{worldUpVersor},
cameraSpeed{speed},
cameraSensitivity{sensitivity}
{updateDirection();}

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

    updateDirection();
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
    cameraPosition += cameraRigthVersor * cameraSpeed;
}
void Camera::moveLeft()
{
    cameraPosition -= cameraRigthVersor * cameraSpeed;
}

void Camera::updateDirection()
{
    direction = glm::normalize(glm::vec3(cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch)),sin(glm::radians(cameraPitch)),sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch))));
    cameraRigthVersor = glm::normalize(glm::cross(direction,worldUpVersor));
    cameraUpVersor = glm::normalize(glm::cross(cameraRigthVersor,direction));
}