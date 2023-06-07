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
camSensitivity{0.1f},
camZoom{45.0f},
camNear{0.1f},
camFar{100.0f}
{}

glm::mat4 myLookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
    glm::vec3 zaxis = glm::normalize(pos - target);
    glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(up), zaxis));
    glm::vec3 yaxis = glm::cross(zaxis, xaxis);

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -pos.x;
    translation[3][1] = -pos.y;
    translation[3][2] = -pos.z;
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = xaxis.x; 
    rotation[1][0] = xaxis.y;
    rotation[2][0] = xaxis.z;
    rotation[0][1] = yaxis.x; 
    rotation[1][1] = yaxis.y;
    rotation[2][1] = yaxis.z;
    rotation[0][2] = zaxis.x; 
    rotation[1][2] = zaxis.y;
    rotation[2][2] = zaxis.z; 
    
    return rotation * translation;
}

glm::mat4 Camera::getWorldToViewTransformationMatrix()
{
    return myLookAt(camPosition, camPosition + camDirection, camUpVersor);
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

void Camera::updateZoom(float deltaZoom)
{
    camZoom -= static_cast<float>(deltaZoom);
    if (camZoom < 1.0f)
        camZoom = 1.0f;
    if (camZoom > 45.0f)
        camZoom = 45.0f; 
}

float Camera::getZoom()
{
    return camZoom;
}

float Camera::getNearVal()
{
    return camNear;
}
float Camera::getFarVal()
{
    return camFar;
}

void Camera::setCameraSpeed(float newSpeed)
{
    camSpeed = newSpeed;
}

float Camera::getCameraSpeed()
{
    return camSpeed;
}

void Camera::setPositionUniform(unsigned int programId,const std::string& name)
{
    glUniform3fv(glGetUniformLocation(programId,name.c_str()), 1, glm::value_ptr(camPosition));
}