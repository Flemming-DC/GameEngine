#include "InputKey.h"
#include "imgui/imgui.h"
#include "ErrorChecker.h"
#include "ListTools.h"
#include "logger.h"
#include "GlfwInput.h"
#include "KeyMap.h"
#include "Screen.h"
#include "Time_.h"
#include "InputAction.h" // its a bit messy that input imports this, since InputAction is a higher level objects than Input

using namespace Key;
Event<bool, unsigned int> InputKey::onGamepadConnectionChanged;


float InputKey::GetFloat(FloatKey key, int gamepadID)
{
    // we turn scrollDelta into scrollVelocity and scale the sensitivity to resemble that of other inputs
    if (key == FloatKey::mouseScrollVelocity)
        return ScrollDelta() / Time::Delta() / 24.0f; 
    return GlfwInput::GamepadFloat(KeyMap::ToGlfw(key), gamepadID);
}


// -------------- Mouse special input --------------

float InputKey::ScrollDelta()
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::GetIO().MouseWheel;
    else
        return GlfwInput::scrollDelta;
}


glm::vec2 InputKey::MouseScreenPosition()
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
glm::vec2 InputKey::NormalizedMouseScreenPosition()
{
    // normalized screen position only uses the game screen, not the editor and therefore not imGUI
    auto mousePos = GlfwInput::MouseScreenPosition();
    return Screen::NormalizeScreenPosition({ mousePos.first, mousePos.second });
}

glm::vec3 InputKey::MouseWorldPosition() { return Screen::ToWorldPosition(NormalizedMouseScreenPosition()); }
glm::vec2 InputKey::MouseWorldPosition2D() { return Screen::ToWorldPosition(NormalizedMouseScreenPosition()); }


// -------------- Gamepad special input --------------


bool InputKey::HasGamepad(int gamepadID) { return GlfwInput::HasGamepad(gamepadID); }
std::vector<unsigned int> InputKey::GamepadIDs() { return GlfwInput::gamepadIDs; }

// -------------- Keyboard: IsPressed, BecomesPressed, BecomesReleased --------------

bool InputKey::IsPressed(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
        return GlfwInput::KeyHeldDown(KeyMap::ToGlfw(key));
}


bool InputKey::BecomesPressed(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key), false);
    else
        return Tools::at_default(GlfwInput::actionByKey, KeyMap::ToGlfw(key), -1) == GLFW_PRESS;
}


bool InputKey::BecomesReleased(Keyboard key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else
        return Tools::at_default(GlfwInput::actionByKey, KeyMap::ToGlfw(key), -1) == GLFW_RELEASE;
}

// -------------- Mouse: IsPressed, BecomesPressed, BecomesReleased --------------

bool InputKey::IsPressed(Mouse key)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyDown(KeyMap::ToImGui(key));
    else
        return GlfwInput::MouseButtonHeldDown(KeyMap::ToGlfw(key));
}


bool InputKey::BecomesPressed(Mouse key)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyPressed(KeyMap::ToImGui(key), false);
    else
        return Tools::at_default(GlfwInput::actionByMouseButton, KeyMap::ToGlfw(key), -1) == GLFW_PRESS;
}


bool InputKey::BecomesReleased(Mouse key)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return ImGui::IsKeyReleased(KeyMap::ToImGui(key));
    else
        return Tools::at_default(GlfwInput::actionByMouseButton, KeyMap::ToGlfw(key), -1) == GLFW_RELEASE;
}

// -------------- Gamepad: IsPressed, BecomesPressed, BecomesReleased --------------

bool InputKey::IsPressed(Gamepad key, int gamepadID)
{
    return GlfwInput::GamepadButtonHeldDown(KeyMap::ToGlfw(key), gamepadID);
}


bool InputKey::BecomesPressed(Gamepad key, int gamepadID)
{
    bool wasDown = GlfwInput::GamepadButtonWasHeldDown(KeyMap::ToGlfw(key), gamepadID);
    bool isDown = GlfwInput::GamepadButtonHeldDown(KeyMap::ToGlfw(key), gamepadID);
    return isDown && !wasDown;
}


bool InputKey::BecomesReleased(Gamepad key, int gamepadID)
{
    bool wasDown = GlfwInput::GamepadButtonWasHeldDown(KeyMap::ToGlfw(key), gamepadID);
    bool isDown = GlfwInput::GamepadButtonHeldDown(KeyMap::ToGlfw(key), gamepadID);
    return !isDown && wasDown;
}


