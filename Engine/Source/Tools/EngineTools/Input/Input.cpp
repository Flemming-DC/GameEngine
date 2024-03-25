#include "Input.h"
#include "ErrorChecker.h"
#include "OpenGlSetup.h"
#include "ListTools.h"
#include "logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

bool Input::glwfScrollHeldDown = false;
int Input::glwfScrollDirection = 0;
std::unordered_map<int, Input::glwfAction> Input::glwfActionByKey;


void Input::Setup()
{
    GLFWwindow* window = OpenGlSetup::GetWindow();
    glCall(glfwFocusWindow(window)); // this should be called by default
    glCall(glfwSetWindowFocusCallback(window, GlfwLogFocusChange));
    glCall(glfwSetKeyCallback(window, GlfwKeyboardCallback));
    glCall(glfwSetScrollCallback(window, GlfwScrollCallback));
}

void Input::LateUpdate()
{
    // resetting input data
    glwfScrollHeldDown = false;
    glwfScrollDirection = 0;
    glwfActionByKey.clear();
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

    if (key == GLFW_KEY_RIGHT_CONTROL)
        key = GLFW_KEY_LEFT_CONTROL; // we don't distinguish right and left ctrl, since ImGUI doesn't do so.

    glwfAction action = (glwfAction)action_;
    glwfActionByKey[key] = (glwfAction)action_;
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
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key), false);
    else
        return Tools::at_default(glwfActionByKey, KeyMap::ToGlfw(key), glwfAction::null) == glwfAction::pressed;
}


bool Input::KeyReleased(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else
        return Tools::at_default(glwfActionByKey, KeyMap::ToGlfw(key), glwfAction::null) == glwfAction::released;
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
