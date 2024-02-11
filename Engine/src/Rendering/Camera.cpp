#include "Camera.h"
#include <GL/glew.h>
#include <string>
#include "ErrorChecker.h"
#include "Time_.h"
#include "Input.h"

// static variables must be initialized in the cpp file not the header file
Camera* Camera::currentCamera = nullptr;

void Camera::OnConstructed()
{
    if (currentCamera == nullptr)
        currentCamera = this;
}


void Camera::SetOrthographicProjection(float scale, float nearClipping, float farClipping)
{
    isProjectionInitialized = true;
    this->nearClipping = nearClipping;

    GLint viewport[4];
    glCall(glGetIntegerv(GL_VIEWPORT, viewport));
    int width = viewport[2];
    int height = viewport[3];
    float aspectRatio = width / (float)height;

    projection = glm::ortho(
        -aspectRatio / scale,
        aspectRatio / scale,
        -1 / scale,
        1 / scale,
        nearClipping,
        farClipping);
}

glm::mat4 Camera::GetProjection() const
{
    if (!isProjectionInitialized)
        RaiseError("projection must be initialized before it can be used.");
    return projection;
}


glm::mat4 Camera::GetView() //const
{
    if (nearClipping > -GetTransform()->GetPosition().z)
        Warning(
            "near clipping plane is closer than minus camera z position, which can cause black screen\n"
            "nearClipping = " + std::to_string(nearClipping) + "\n"
            "camera position.z = " + std::to_string(GetTransform()->GetPosition().z));

    return GetTransform()->GetInverseModel();
}


void Camera::OnUpdate()
{
    glm::vec3 direction = glm::vec3(0.0f);
    if (Input::GetKey(GLFW_KEY_A))
        direction.x -= 1;
    if (Input::GetKey(GLFW_KEY_D))
        direction.x += 1;
    if (Input::GetKey(GLFW_KEY_S))
        direction.y -= 1;
    if (Input::GetKey(GLFW_KEY_W))
        direction.y += 1;
    if (direction.x != 0 && direction.y != 0)
        direction = glm::normalize(direction);

    GetTransform()->localPosition += direction * moveSpeed * Time::GetDelta();
}



Camera* Camera::GetCurrent()
{
    if (currentCamera == nullptr)
        RaiseError("currentCamera is null. If you destroy the current camera, then you must set a new camera as the current one.");
    return currentCamera;
}
