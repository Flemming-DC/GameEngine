#include "Input.h"
#include "imgui/imgui.h"
#include "ErrorChecker.h"
#include "ListTools.h"
#include "logger.h"
#include "GlfwInput.h"


void Input::Setup() { GlfwInput::Setup(); }
void Input::LateUpdate() { GlfwInput::LateUpdate(); }


bool Input::KeyHeldDown(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
        return GlfwInput::KeyHeldDown(KeyMap::ToGlfw(key));
}


bool Input::KeyPressed(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key), false);
    else
        return Tools::at_default(GlfwInput::actionByKey, KeyMap::ToGlfw(key), -1) == GLFW_PRESS;
}


bool Input::KeyReleased(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else
        return Tools::at_default(GlfwInput::actionByKey, KeyMap::ToGlfw(key), -1) == GLFW_RELEASE;
}


bool Input::GetScrollHeldDown()
{ 
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyDown(ImGuiKey_MouseMiddle);
    else
        return GlfwInput::scrollHeldDown;
}

int Input::GetScrollDirection()
{ 
    if (ImGui::GetIO().WantCaptureMouse)
        return (int)ImGui::GetIO().MouseWheel;
    else
        return GlfwInput::scrollDirection;
}
