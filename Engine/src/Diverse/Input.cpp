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

bool Input::KeyHeldDown(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
    {
        glCall(int state = glfwGetKey(window, KeyMap::ToGlfw(key)));
        return state == GLFW_PRESS;
    }
}


ImGuiKey Input::GetImGuiKey(int glfwKey) 
{
    switch (glfwKey)
    {
    case GLFW_KEY_SPACE: return ImGuiKey_Space;
    case GLFW_KEY_A: return ImGuiKey_A;
    case GLFW_KEY_B: return ImGuiKey_B;
    case GLFW_KEY_C: return ImGuiKey_C;
    case GLFW_KEY_D: return ImGuiKey_D;
    case GLFW_KEY_E: return ImGuiKey_E;
    case GLFW_KEY_F: return ImGuiKey_F;
    case GLFW_KEY_G: return ImGuiKey_G;
    case GLFW_KEY_H: return ImGuiKey_H;
    case GLFW_KEY_I: return ImGuiKey_I;
    case GLFW_KEY_J: return ImGuiKey_J;
    case GLFW_KEY_K: return ImGuiKey_K;
    case GLFW_KEY_L: return ImGuiKey_L;
    case GLFW_KEY_M: return ImGuiKey_M;
    case GLFW_KEY_N: return ImGuiKey_N;
    case GLFW_KEY_O: return ImGuiKey_O;
    case GLFW_KEY_P: return ImGuiKey_P;
    case GLFW_KEY_Q: return ImGuiKey_Q;
    case GLFW_KEY_R: return ImGuiKey_R;
    case GLFW_KEY_S: return ImGuiKey_S;
    case GLFW_KEY_T: return ImGuiKey_T;
    case GLFW_KEY_U: return ImGuiKey_U;
    case GLFW_KEY_V: return ImGuiKey_V;
    case GLFW_KEY_W: return ImGuiKey_W;
    case GLFW_KEY_X: return ImGuiKey_X;
    case GLFW_KEY_Y: return ImGuiKey_Y;
    case GLFW_KEY_Z: return ImGuiKey_Z;
    //case GLFW_KEY_: return ImGuiKey_;
    default:
        RaiseError("Missing key");
        return ImGuiKey_COUNT; // Unknown key
    }
}
