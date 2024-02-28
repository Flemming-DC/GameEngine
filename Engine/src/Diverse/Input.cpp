#include "Input.h"
#include "ErrorChecker.h"
#include "Initializer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool Input::glwfScrollHeldDown = false;
int Input::glwfScrollDirection = 0;

void Input::Setup()
{
    GLFWwindow* window = Initializer::GetWindow();
    glCall(glfwFocusWindow(window)); // this should be called by default
    glCall(glfwSetWindowFocusCallback(window, GlfwLogFocusChange));
    glCall(glfwSetKeyCallback(window, GlfwKeyboardCallback));
    glCall(glfwSetScrollCallback(window, GlfwScrollCallback));
}

void Input::Update()
{
    // resetting input data
    glwfScrollHeldDown = false;
    glwfScrollDirection = 0;
}


void Input::GlfwLogFocusChange(GLFWwindow* window, int focused)
{
    if (focused == GLFW_FALSE)
        Log("glfw Lost focus");
    else if (focused == GLFW_TRUE)
        Log("glfw Gained focus");
}


void Input::GlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if you implement an event type, then you can invoke it here with (key, action, mods) as input
    // or perhaps listen for specific key, action, mods combination rather than receiving this as parameters
}

void Input::GlfwScrollCallback(GLFWwindow* window, double pressed, double direction)
{
    glwfScrollHeldDown = (pressed == 1);
    glwfScrollDirection = (int)direction;
}

bool Input::KeyHeldDown(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
    {
        glCall(int state = glfwGetKey(Initializer::GetWindow(), KeyMap::ToGlfw(key)));
        return state == GLFW_PRESS;
    }
}


bool Input::GetScrollHeldDown()
{ 
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyDown(ImGuiKey_MouseMiddle);
    else
        return glwfScrollHeldDown; 
}

int Input::GetScrollDirection()
{ 
    if (ImGui::GetIO().WantCaptureMouse)
        return (int)ImGui::GetIO().MouseWheel;
    else
        return glwfScrollDirection; 
}
