#include "Camera.h"
#include <GL/glew.h>
#include <string>
#include "OpenGlError.h"
#include "Time_.h"
#include "InputKey.h"

Shorts
Camera* Camera::currentCamera = nullptr;

void Camera::OnStart()
{
    if (currentCamera == nullptr)
        currentCamera = this;
    SetToOrthographic(); // this is hardcoded, since Im only aiming for a 2D game
}

void Camera::OnDestroy()
{
    if (currentCamera == this)
        currentCamera = nullptr;
}


void Camera::SetToOrthographic(float scale, float nearClipping, float farClipping)
{
    isProjectionInitialized = true;
    this->nearClipping = nearClipping;
    projection = MakeOrthoProjection();
}

mat4 Camera::MakeOrthoProjection(float scale, float nearClipping, float farClipping)
{
    GLint viewport[4];
    glCall(glGetIntegerv(GL_VIEWPORT, viewport));
    int width = viewport[2];
    int height = viewport[3];
    float aspectRatio = width / (float)height;

    return glm::ortho(
        -aspectRatio / scale,
        aspectRatio / scale,
        -1 / scale,
        1 / scale,
        nearClipping,
        farClipping);
}

glm::mat4 Camera::Projection() const
{
    if (!isProjectionInitialized)
        RaiseError("projection must be initialized before it can be used.");
    return projection;
}


glm::mat4 Camera::View() const
{
    if (nearClipping > -GetTransform().GetPosition().z)
        Warning(
            "near clipping plane is closer than minus camera z position, which can cause black screen\n"
            "nearClipping = " + std::to_string(nearClipping) + "\n"
            "camera position.z = " + std::to_string(GetTransform().GetPosition().z));

    return GetTransform().GetInverseModel();
}

mat4 Camera::ProjectionView(vec3 cameraPos, quat cameraRot, vec3 cameraScale)
{
    mat4 view = glm::scale(glm::mat4(1.0f), 1.0f / cameraScale)
        * glm::mat4_cast(glm::inverse(cameraRot))
        * glm::translate(glm::mat4(1.0f), -cameraPos);

    return MakeOrthoProjection() * view;
}




Camera& Camera::Current()
{
    if (currentCamera == nullptr)
        RaiseError("currentCamera is null. If you destroy the current camera, then you must set a new camera as the current one.");
    return *currentCamera;
}
