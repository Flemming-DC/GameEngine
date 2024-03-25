#include "Input.h"
#include "ErrorChecker.h"
#include "OpenGlSetup.h"
#include "logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool Input::glwfScrollHeldDown = false;
int Input::glwfScrollDirection = 0;
enum class glwfAction {pressed, released, heldDownLong}; // evt. add heldDown and make it into a general action class

void Input::Setup()
{
    GLFWwindow* window = OpenGlSetup::GetWindow();
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
        logger::print("glfw Lost focus");
    else if (focused == GLFW_TRUE)
        logger::print("glfw Gained focus");
}


void Input::GlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action_, int mods)
{
    // if you implement an event type, then you can invoke it here with (key, action, mods) as input
    // or perhaps listen for specific key, action, mods combination rather than receiving this as parameters

    glwfAction action = (glwfAction)action_;
    if (key == GLFW_KEY_Q)
    {
        P("GLFW_KEY_Q: ", key, " ", scancode, " ", action_, " ", mods);
    }
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
    else if (key == Keyboard::ctrl)
    {
        glCall(int leftCtrl = glfwGetKey(OpenGlSetup::GetWindow(), GLFW_KEY_LEFT_CONTROL));
        glCall(int rightCtrl = glfwGetKey(OpenGlSetup::GetWindow(), GLFW_KEY_RIGHT_CONTROL));
        return leftCtrl == GLFW_PRESS || rightCtrl == GLFW_PRESS;
    }
    else
    {
        glCall(int state = glfwGetKey(OpenGlSetup::GetWindow(), KeyMap::ToGlfw(key)));
        return state == GLFW_PRESS;
    }
}


bool Input::KeyPressed(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key));
    else if (key == Keyboard::ctrl)
    {
        return false;
    }
    else
    {
        return false;
    }
}


bool Input::KeyReleased(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else if (key == Keyboard::ctrl)
    {
        return false;
    }
    else
    {
        return false;
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
