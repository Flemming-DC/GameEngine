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
        glCall(glfwSetInputMode(OpenGlSetup::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN)); 
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


vec3 Screen::ToWorldPosition(vec2 screenPos, bool* foundDepth)
{
    mat4 projView = Camera::Current().ProjectionView();
    return ToWorldPosition(screenPos, projView, foundDepth);
}
vec2 Screen::FromWorldPosition(vec3 worldPosition)
{
    mat4 projView = Camera::Current().ProjectionView();
    return FromWorldPosition(worldPosition, projView);
}
vec3 Screen::ToWorldPosition(
    vec2 screenPos, vec2 screenMinCorner, vec2 screenMaxCorner, 
    vec3 cameraPos, quat cameraRot, vec3 cameraScale, bool* foundDepth)
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
vec2 Screen::FromWorldPosition(vec3 worldPosition, vec3 cameraPos, quat cameraRot, vec3 cameraScale)
{
    mat4 projView = Camera::ProjectionView(cameraPos, cameraRot, cameraScale);
    return FromWorldPosition(worldPosition, projView);
}


vec3 Screen::ToWorldPosition(vec2 screenPos, mat4 projectionView, bool* foundDepth)
{
    // find depth
    float depth;
    int pixel_x = (int)std::round(screenPos.x);
    int pixel_y = (int)std::round(screenPos.y);
    // obtain the Z component. It should be in the interval [0, 1] 
    glCall(glReadPixels(pixel_x, pixel_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth)); 
    if (foundDepth != nullptr)
        *foundDepth = (depth > -0.0001f && depth < 1.0001f);
    depth = glm::clamp(depth, 0.0f, 1.0f);
    vec3 screenPos3D(screenPos.x, screenPos.y, depth);

    vec4 viewport = vec4(0.0f, 0.0f, OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    // unProject expect a view, projection, where we give it a identity, projectionView, but its ok, 
    // since it anyhow only uses this data to construct the projectionView
    vec3 worldPos = glm::unProject(screenPos3D, mat4(1.0f), projectionView, viewport); 

    return worldPos;
}

vec2 Screen::FromWorldPosition(vec3 worldPosition, mat4 projectionView)
{
    vec4 viewport = vec4(0.0f, 0.0f, OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    // unProject expect a view, projection, where we give it a identity, projectionView, but its ok, 
    // since it anyhow only uses this data to construct the projectionView
    vec2 screenPos = glm::project(worldPosition, mat4(1.0f), projectionView, viewport);
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


