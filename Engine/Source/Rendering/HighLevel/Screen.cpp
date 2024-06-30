#include "Screen.h"
#include "Camera.h"
#include "OpenGlSetup.h"

Shorts;
CursorMode Screen::cursorMode = CursorMode::normal;


void Screen::ApplyCursorState()
{
    if (cursorMode == CursorMode::hidden)
    {
        // glfw unhides the cursor unless, we keep hiding it.
        glfwSetInputMode(OpenGlSetup::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
}


void Screen::SetCursorMode(CursorMode cursorMode_)
{
    cursorMode = cursorMode_;
    switch (cursorMode_)
    {
    case CursorMode::normal: glCall(glfwSetInputMode(OpenGlSetup::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL)); break;
    case CursorMode::hidden: glCall(glfwSetInputMode(OpenGlSetup::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN)); break;
    case CursorMode::locked: glCall(glfwSetInputMode(OpenGlSetup::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED)); break;
    default:
        RaiseError("unrecognized cursorMode ", cursorMode_);
    }

}

void Screen::SetTitle(std::string gameTitle)
{
    glCall(glfwSetWindowTitle(OpenGlSetup::GetWindow(), gameTitle.c_str()));
}



// --------------- world/screen position conversions ---------------


vec3 Screen::ToWorldPosition(
    vec2 screenPos, vec2 screenMinCorner, vec2 screenMaxCorner, 
    vec3 cameraPos, quat cameraRot, vec3 cameraScale)
{
    vec2 halfDelta = vec2((screenMaxCorner.x - screenMinCorner.x) / 2.0f, 
                          (screenMaxCorner.y - screenMinCorner.y) / 2.0f);
    vec2 center = vec2((screenMaxCorner.x + screenMinCorner.x) / 2.0f,
                       (screenMaxCorner.y + screenMinCorner.y) / 2.0f);

    vec2 clampedScreenPos = {
        glm::clamp(screenPos.x, screenMinCorner.x, screenMaxCorner.x),
        glm::clamp(screenPos.y, screenMinCorner.y, screenMaxCorner.y) };
    vec2 normalizedScreenPos = {
        (clampedScreenPos.x - center.x) / halfDelta.x,
       -(clampedScreenPos.y - center.y) / halfDelta.y };
    
    vec4 screenPos4D(
        normalizedScreenPos.x,
        normalizedScreenPos.y,
        0.5f,
        1.0f);

    mat4 projectionView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);
    mat4 inverseProjectionView = glm::inverse(projectionView);

    vec4 worldPos4D = inverseProjectionView * screenPos4D;
    vec3 worldPos3D = vec3(worldPos4D.x, worldPos4D.y, worldPos4D.z);

    return worldPos3D;
}

vec2 Screen::FromWorldPosition(
    vec3 worldPosition, vec2 screenMinCorner, vec2 screenMaxCorner,
    vec3 cameraPos, quat cameraRot, vec3 cameraScale)
{
    // setup required info
    vec2 halfDelta = vec2((screenMaxCorner.x - screenMinCorner.x) / 2.0f,
        (screenMaxCorner.y - screenMinCorner.y) / 2.0f);
    vec2 center = vec2((screenMaxCorner.x + screenMinCorner.x) / 2.0f,
        (screenMaxCorner.y + screenMinCorner.y) / 2.0f);
    mat4 projView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);

    // do calculation
    vec4 worldPos4D = vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);
    vec4 normalizedScreenPos4D = projView * worldPos4D;
    vec2 normalizedScreenPos2D = vec2(normalizedScreenPos4D.x, -normalizedScreenPos4D.y); // note the minus sign
    vec2 screenPos = center + normalizedScreenPos2D * halfDelta; // elementwise multiplication
    return screenPos; 
    /*
    mat4 projView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);
    vec4 viewport = vec4(screenMinCorner.x, screenMinCorner.y, screenMaxCorner.x, screenMaxCorner.y);
    // unProject expect a view, projection, where we give it a identity, projectionView, but its ok, 
    // since it anyhow only uses this data to construct the projectionView
    vec2 screenPos = glm::project(worldPosition, mat4(1.0f), projView, viewport);
    screenPos.y = screenMaxCorner.y - screenPos.y;
    return screenPos;
    */
}


vec3 Screen::ToWorldPosition(vec2 somewhatNormalizedScreenPos)
{
    vec2 center = vec2(OpenGlSetup::GetWidth() / 2.0f,
                       OpenGlSetup::GetHeight() / 2.0f);
    vec2 halfDelta = center; // since screenMinCorner = vec(0, 0) then halfDelta = center
    
    vec2 normalizedScreenPos2 = {
        (somewhatNormalizedScreenPos.x - center.x) / halfDelta.x,
        (somewhatNormalizedScreenPos.y - center.y) / halfDelta.y }; // dont flip y. This has already been done

    vec4 screenPos4D = {
        normalizedScreenPos2.x,
        normalizedScreenPos2.y,
        0.5f,
        1.0f };

    mat4 projectionView = Camera::Current().ProjectionView();
    mat4 inverseProjectionView = glm::inverse(projectionView);

    vec4 worldPos4D = inverseProjectionView * screenPos4D;
    vec3 worldPos3D = vec3(worldPos4D.x, worldPos4D.y, worldPos4D.z);

    return worldPos3D;
}

vec2 Screen::FromWorldPosition(vec3 worldPosition)
{
    vec4 viewport = vec4(0.0f, 0.0f, OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    mat4 projView = Camera::Current().ProjectionView();
    // unProject expect a view, projection, where we give it a identity, projectionView, but its ok, 
    // since it anyhow only uses this data to construct the projectionView
    vec2 screenPos = glm::project(worldPosition, mat4(1.0f), projView, viewport);
    return screenPos;
}

vec2 Screen::NormalizeGameScreenPosition(vec2 screenPos)
{
    float width = (float)OpenGlSetup::GetWidth();
    float height = (float)OpenGlSetup::GetHeight();
    screenPos = {
        glm::clamp(screenPos.x, 0.0f, width),
        glm::clamp(screenPos.y, 0.0f, height) };
    screenPos.y = height - screenPos.y; // Invert Y coordinate
    return screenPos;
}


