#include "Input.h"
#include "imgui/imgui.h"
#include "ErrorChecker.h"
#include "ListTools.h"
#include "logger.h"
#include "GlfwInput.h"
#include "KeyMap.h"
#include "Screen.h"
#include "InputAction.h" // its a bit messy that input imports this, since InputAction is a higher level objects than Input

const unsigned int Input::findSinglePlayerGamepad = GlfwInput::findSinglePlayerGamepad;
Event<bool, unsigned int> Input::onGamepadConnectionChanged;


void Input::Setup() 
{ 
    GlfwInput::Setup(); 
}

void Input::LateUpdate() 
{ 
    GlfwInput::LateUpdate();
    Screen::LateUpdate();
    InputAction<bool>::LateUpdate();
    InputAction<float>::LateUpdate();
    InputAction<glm::vec2>::LateUpdate();
}


// -------------- Mouse special input --------------

int Input::GetScrollDirection()
{
    if (ImGui::GetIO().WantCaptureMouse)
        return (int)ImGui::GetIO().MouseWheel;
    else
        return GlfwInput::scrollDirection;
}


glm::vec2 Input::MouseScreenPosition()
{
    if (ImGui::GetIO().WantCaptureMouse)
    {
        auto mousePos = ImGui::GetMousePos();
        return Screen::NormalizeScreenPosition({ mousePos.x, mousePos.y});
    }
    else
    {
        auto mousePos = GlfwInput::MouseScreenPosition();
        return Screen::NormalizeScreenPosition({ mousePos.first, mousePos.second });
    }
}
glm::vec2 Input::NormalizedMouseScreenPosition()
{
    // normalized screen position only uses the game screen, not the editor and therefore not imGUI
    auto mousePos = GlfwInput::MouseScreenPosition();
    return Screen::NormalizeScreenPosition({ mousePos.first, mousePos.second });
}

glm::vec3 Input::MouseWorldPosition() { return Screen::ToWorldPosition(NormalizedMouseScreenPosition()); }
glm::vec2 Input::MouseWorldPosition2D() { return Screen::ToWorldPosition(NormalizedMouseScreenPosition()); }


// -------------- Gamepad special input --------------


float Input::GetFloat(FloatKey key, int gamepadID)
{
    if (key == FloatKey::mouseScrollDirection)
        return (float)GetScrollDirection();
    return GlfwInput::GamepadFloat(KeyMap::ToGlfw(key), gamepadID);
}

bool Input::HasGamepad(int gamepadID) { return GlfwInput::HasGamepad(gamepadID); }
std::vector<unsigned int> Input::GamepadIDs() { return GlfwInput::gamepadIDs; }

// -------------- Keyboard: IsHeldDown, IsPressed, IsReleased --------------

bool Input::IsHeldDown(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
        return GlfwInput::KeyHeldDown(KeyMap::ToGlfw(key));
}


bool Input::IsPressed(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key), false);
    else
        return Tools::at_default(GlfwInput::actionByKey, KeyMap::ToGlfw(key), -1) == GLFW_PRESS;
}


bool Input::IsReleased(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else
        return Tools::at_default(GlfwInput::actionByKey, KeyMap::ToGlfw(key), -1) == GLFW_RELEASE;
}

// -------------- Mouse: IsHeldDown, IsPressed, IsReleased --------------

bool Input::IsHeldDown(Mouse key)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
        return GlfwInput::MouseButtonHeldDown(KeyMap::ToGlfw(key));
}


bool Input::IsPressed(Mouse key)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key), false);
    else
        return Tools::at_default(GlfwInput::actionByMouseButton, KeyMap::ToGlfw(key), -1) == GLFW_PRESS;
}


bool Input::IsReleased(Mouse key)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else
        return Tools::at_default(GlfwInput::actionByMouseButton, KeyMap::ToGlfw(key), -1) == GLFW_RELEASE;
}

// -------------- Gamepad: IsHeldDown, IsPressed, IsReleased --------------

bool Input::IsHeldDown(Gamepad key, int gamepadID)
{
    return GlfwInput::GamepadButtonHeldDown(KeyMap::ToGlfw(key), gamepadID);
}


bool Input::IsPressed(Gamepad key, int gamepadID)
{
    bool wasDown = GlfwInput::GamepadButtonWasHeldDown(KeyMap::ToGlfw(key), gamepadID);
    bool isDown = GlfwInput::GamepadButtonHeldDown(KeyMap::ToGlfw(key), gamepadID);
    return isDown && !wasDown;
}


bool Input::IsReleased(Gamepad key, int gamepadID)
{
    bool wasDown = GlfwInput::GamepadButtonWasHeldDown(KeyMap::ToGlfw(key), gamepadID);
    bool isDown = GlfwInput::GamepadButtonHeldDown(KeyMap::ToGlfw(key), gamepadID);
    return !isDown && wasDown;
}


