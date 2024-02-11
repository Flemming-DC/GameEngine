#include "Input.h"
#include "ErrorChecker.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static GLFWwindow* window;
bool Input::scrollHeldDown = false;
int Input::scrollDirection = 0;

void Input::Setup(GLFWwindow* window_)
{
    window = window_;
    glCall(glfwFocusWindow(window)); // this should be called by default
    glCall(glfwSetWindowFocusCallback(window, LogFocusChange));
    glCall(glfwSetKeyCallback(window, KeyboardCallback));
    glCall(glfwSetScrollCallback(window, ScrollCallback));
}

void Input::Update()
{
    // resetting input data
    scrollHeldDown = false;
    scrollDirection = 0;
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

void Input::ScrollCallback(GLFWwindow* window, double pressed, double direction)
{
    scrollHeldDown = (pressed == 1);
    scrollDirection = (int)direction;
}

bool Input::KeyPressed(int key)
{
    glCall(int state = glfwGetKey(window, key));
    if (state == GLFW_PRESS)
        return true;
    else
        return false;
}



