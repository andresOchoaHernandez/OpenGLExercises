#include <camera.hpp>

Camera::Camera():
worldUpVersor{glm::vec3(0.0f,1.0f,0.0f)},
camPitch{0.0f},
camYaw{-90.0f},
camPosition{glm::vec3(0.0f,0.0f,6.0f)},
camDirection{glm::normalize(glm::vec3(cos(glm::radians(camYaw)) * cos(glm::radians(camPitch)),sin(glm::radians(camPitch)),sin(glm::radians(camYaw)) * cos(glm::radians(camPitch))))},
camUpVersor{glm::vec3(0.0f,1.0f,0.0f)},
camRigthVersor{glm::normalize(glm::cross(camDirection, worldUpVersor))},
camSpeed{0.2f},
camSensitivity{0.1f}
{}

glm::mat4 Camera::getWorldToViewTransformationMatrix()
{
    return glm::lookAt(camPosition, camPosition + camDirection, camUpVersor);
}
void Camera::updatePitchAndYaw(float xoffset, float yoffset)
{
    xoffset *= camSensitivity;
    yoffset *= camSensitivity;

    camYaw   += xoffset;
    camPitch += yoffset;

    if(camPitch > 89.0f)
        camPitch = 89.0f;
    if(camPitch < -89.0f)
        camPitch = -89.0f;

    updateDirection();
}

glm::vec3 Camera::getCamPosition()
{
    return camPosition;
}

void Camera::moveForward()
{
    camPosition += camSpeed * camDirection;
}
void Camera::moveBack()
{
    camPosition -= camSpeed * camDirection;
}
void Camera::moveRigth()
{
    camPosition += camRigthVersor * camSpeed;
}
void Camera::moveLeft()
{
    camPosition -= camRigthVersor * camSpeed;
}

void Camera::updateDirection()
{
    camDirection = glm::normalize(glm::vec3(cos(glm::radians(camYaw)) * cos(glm::radians(camPitch)),sin(glm::radians(camPitch)),sin(glm::radians(camYaw)) * cos(glm::radians(camPitch))));
    camRigthVersor = glm::normalize(glm::cross(camDirection,worldUpVersor));
    camUpVersor = glm::normalize(glm::cross(camRigthVersor,camDirection));
    
}