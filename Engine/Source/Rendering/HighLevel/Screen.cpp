#include "Screen.h"
#include "Camera.h"
#include "OpenGlSetup.h"


Shorts;

vec3 Screen::ToWorldPosition(vec2 screenPos, bool* foundDepth)
{
    // find depth
    float depth;
    glCall(glReadPixels(screenPos.x, screenPos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth)); // obtain the Z component. It should be in the interval [0, 1] 
    if (foundDepth != nullptr)
        *foundDepth = (depth > -0.0001f && depth < 1.0001f);
    depth = glm::clamp(depth, 0.0f, 1.0f);
    vec3 screenPos3D(screenPos.x, screenPos.y, depth);

    vec4 viewport = vec4(0.0f, 0.0f, OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    mat4 projection = Camera::GetCurrent().GetProjection();
    mat4 view = Camera::GetCurrent().GetView();
    vec3 worldPos = glm::unProject(screenPos3D, view, projection, viewport);

    return worldPos;
}

vec2 Screen::FromWorldPosition(vec3 worldPosition)
{
    vec4 viewport = vec4(0.0f, 0.0f, OpenGlSetup::GetWidth(), OpenGlSetup::GetHeight());
    mat4 projection = Camera::GetCurrent().GetProjection();
    mat4 view = Camera::GetCurrent().GetView();
    
    vec2 screenPos = glm::project(worldPosition, view, projection, viewport);

    return screenPos;
}


vec2 Screen::NormalizeScreenPosition(vec2 screenPos)
{
    float width = OpenGlSetup::GetWidth();
    float height = OpenGlSetup::GetHeight();
    screenPos = {
        glm::clamp(screenPos.x, 0.0f, width),
        glm::clamp(screenPos.y, 0.0f, height) };
    screenPos.y = height - screenPos.y; // Invert Y coordinate
    return screenPos;
}