#include "Screen.h"
#include "Camera.h"
#include "OpenGlSetup.h"

// figure out winX, winY, winZ, screenPos are really necessary
// double vs float
// check if mouse is in window, and modify error accordingly
// replace std::pair with glm::vec2
// construct FromWorldPosition

glm::vec3 Screen::ToWorldPosition(std::pair<double, double> screenPosition) // screenPosition or MousePosition
{
    double winX, winY;
    float winZ;
    auto window = OpenGlSetup::GetWindow();
    int width = OpenGlSetup::GetWidth();
    int height = OpenGlSetup::GetHeight();

    glCall(glfwGetWindowSize(window, &width, &height));
    winX = screenPosition.first;
    winY = (double)height - screenPosition.second; // Invert Y coordinate
    glCall(glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ)); // obtain the Z component. It should be in the interval [0, 1] 
    P("winZ", winZ);
    if (winZ < -0.001 || winZ > 1.001)
        RaiseError("Failed to obtain a valid value for the depth component");

    glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);
    glm::mat4 projection = Camera::GetCurrent().GetProjection();
    glm::mat4 view = Camera::GetCurrent().GetView();
    glm::vec3 screenPos(winX, winY, winZ); // from screenPosition to screenPos ???
    glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);


    return worldPos;
}

std::pair<double, double> Screen::FromWorldPosition(glm::vec3 worldPosition)
{
    RaiseError("not implemented");
    return {0, 0};
}
