#include "GlfwInput.h"
#include "ErrorChecker.h"
#include "OpenGlSetup.h" // including v is messy, since it violates the principle that tools mustn't depend on systems
#include "ListTools.h"
#include "logger.h"


bool GlfwInput::scrollHeldDown = false;
int GlfwInput::scrollDirection = 0;
std::unordered_map<int, int> GlfwInput::actionByKey;


void GlfwInput::Setup()
{
    GLFWwindow* window = OpenGlSetup::GetWindow();
    glCall(glfwFocusWindow(window));
    glCall(glfwSetWindowFocusCallback(window, logFocusChange));
    glCall(glfwSetKeyCallback(window, KeyboardCallback));
    glCall(glfwSetScrollCallback(window, ScrollCallback));
}

void GlfwInput::LateUpdate()
{
    // resetting input data
    scrollHeldDown = false;
    scrollDirection = 0;
    actionByKey.clear();
}


void GlfwInput::logFocusChange(GLFWwindow* window, int focused)
{
    if (focused == GLFW_FALSE)
        logger::print("glfw Lost focus");
    else if (focused == GLFW_TRUE)
        logger::print("glfw Gained focus");
}


void GlfwInput::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action_, int mods)
{
    // if you implement an event type, then you can invoke it here with (key, action, mods) as input
    // or perhaps listen for specific key, action, mods combination rather than receiving this as parameters
    // action can be 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT 
    if (key == GLFW_KEY_Q)
        P(action_);
    if (key == GLFW_KEY_RIGHT_CONTROL)
        key = GLFW_KEY_LEFT_CONTROL; // we don't distinguish right and left ctrl, since ImGUI doesn't do so.

    //glwfAction action = (glwfAction)action_;
    actionByKey[key] = action_;
}

void GlfwInput::ScrollCallback(GLFWwindow* window, double pressed, double direction)
{
    scrollHeldDown = (pressed == 1);
    scrollDirection = (int)direction;
}


bool GlfwInput::KeyHeldDown(int key)
{
    auto window = OpenGlSetup::GetWindow();
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
    {
        glCall(int leftCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL));
        glCall(int rightCtrl = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL));
        return leftCtrl == GLFW_PRESS || rightCtrl == GLFW_PRESS;
    }
    
    glCall(int state = glfwGetKey(OpenGlSetup::GetWindow(), key));
    return state == GLFW_PRESS;

}

