#include "Camera.h"
#include <GL/glew.h>
#include <string>
#include "ErrorChecker.h"
#include "Time_.h"
#include "Input.h"

// static variables must be initialized in the cpp file not the header file
Camera* Camera::currentCamera = nullptr;

void Camera::OnStart()
{
    if (currentCamera == nullptr)
        currentCamera = this;
    SetToOrthographic(); // this is hardcoded, since Im only aiming for a 2D game
}


void Camera::SetToOrthographic(float scale, float nearClipping, float farClipping)
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
    if (nearClipping > -GetTransform().GetPosition().z)
        Warning(
            "near clipping plane is closer than minus camera z position, which can cause black screen\n"
            "nearClipping = " + std::to_string(nearClipping) + "\n"
            "camera position.z = " + std::to_string(GetTransform().GetPosition().z));

    return GetTransform().GetInverseModel();
}


void Camera::OnUpdate()
{
    float scaling = 1 + Input::GetScrollDirection() * scrollSpeed * Time::GetDelta(); // exp(x) = 1 + x + O(x^2) is used
    projection = glm::scale(projection, glm::vec3(scaling));



    glm::vec3 moveDirection = glm::vec3(0.0f);
    if (Input::KeyHeldDown(Keyboard::A))
        moveDirection.x -= 1;
    if (Input::KeyHeldDown(Keyboard::D))
        moveDirection.x += 1;
    if (Input::KeyHeldDown(Keyboard::S))
        moveDirection.y -= 1;
    if (Input::KeyHeldDown(Keyboard::W))
        moveDirection.y += 1;
    if (moveDirection.x != 0 && moveDirection.y != 0)
        moveDirection = glm::normalize(moveDirection);

    moveSpeed /= scaling; // large scaling means stuff looks bigger, which means zooming in.
    GetTransform().localPosition += moveDirection * moveSpeed * Time::GetDelta();
}



Camera* Camera::GetCurrent()
{
    if (currentCamera == nullptr)
        RaiseError("currentCamera is null. If you destroy the current camera, then you must set a new camera as the current one.");
    return currentCamera;
}
