#include "Input.h"
#include "ErrorChecker.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static GLFWwindow* window;

void Input::Setup(GLFWwindow* window_)
{
    window = window_;
    glCall(glfwFocusWindow(window)); // this should be called by default
    glCall(glfwSetWindowFocusCallback(window, LogFocusChange));
    glCall(glfwSetKeyCallback(window, KeyboardCallback));

}

void Input::LogFocusChange(GLFWwindow* window, int focused)
{
    if (focused == GLFW_FALSE)
        Log("Lost focus");
    else if (focused == GLFW_TRUE)
        Log("Gained focus");
}


void Input::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if you implement an event type, then you can invoke it here with (key, action, mods) as input
    // or perhaps listen for specific key, action, mods combination rather than receiving this as parameters
}

bool Input::GetKey(int key)
{
    glCall(int state = glfwGetKey(window, key));
    if (state == GLFW_PRESS)
        return true;
    else
        return false;
}



