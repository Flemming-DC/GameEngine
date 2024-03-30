#include "GlfwInput.h"
#include "ErrorChecker.h"
#include "OpenGlSetup.h" 
#include "ListTools.h"
#include "logger.h"
#include "InputKey.h" // letting GlfwInput refer to InputKey is dirty, but neessary in order to forward events
#include "GlmTools.h"

float GlfwInput::scrollDirection = 0;
std::unordered_map<int, int> GlfwInput::actionByKey;
std::unordered_map<int, int> GlfwInput::actionByMouseButton;
std::vector<unsigned int> GlfwInput::gamepadIDs;
std::unordered_map<unsigned, GLFWgamepadstate> GlfwInput::lastGamepadStateByJoystick;


void GlfwInput::Setup()
{
    GLFWwindow* window = OpenGlSetup::GetWindow();
    glCall(glfwFocusWindow(window));
    glCall(glfwSetWindowFocusCallback(window, _FocusChangeCallBack));
    glCall(glfwSetKeyCallback(window, _KeyboardCallback));
    glCall(glfwSetMouseButtonCallback(window, _MouseButtonCallback));
    glCall(glfwSetScrollCallback(window, _ScrollCallback));
    glCall(glfwSetJoystickCallback(_JoystickCallback));

    for (int g = 0; g < GLFW_JOYSTICK_LAST; g++)
    {
        if (HasGamepad(g))
        {
            gamepadIDs.push_back((unsigned int)g);
            InputKey::onGamepadConnectionChanged.Invoke(true, g);
        }
    }
}

void GlfwInput::LateUpdate()
{
    // calculate lastGamepadStateByJoystick to be used in the next frame.
    lastGamepadStateByJoystick.clear();
    for (const auto& g : gamepadIDs)
    {
        GLFWgamepadstate state;
        glCall(bool found = glfwGetGamepadState(g, &state));
        if (found)
            lastGamepadStateByJoystick[g] = state;
    }
    
    // resetting input data
    scrollDirection = 0;
    actionByKey.clear();
    actionByMouseButton.clear();
}

bool GlfwInput::HasGamepad(int gamepadID)
{
    if (gamepadID == _singlePlayerGamepadID)
        return !gamepadIDs.empty();

    // checks whether joystick is present and is gamepad. 
    // If we only use gamepads, then this in effect checks whether you have a gamepad or not.
    glCall(bool isGamePad = glfwJoystickIsGamepad(gamepadID));
    return isGamePad;
}

// ------------------ get input (key/button held down) -------------------

bool GlfwInput::KeyHeldDown(int key)
{
    auto window = OpenGlSetup::GetWindow();
    if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
    {
        glCall(int leftCtrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL));
        glCall(int rightCtrl = glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL));
        return leftCtrl == GLFW_PRESS || rightCtrl == GLFW_PRESS;
    }

    glCall(int state = glfwGetKey(window, key));
    return state == GLFW_PRESS;

}

bool GlfwInput::MouseButtonHeldDown(int button)
{
    auto window = OpenGlSetup::GetWindow();
    glCall(int state = glfwGetMouseButton(window, button));
    return state == GLFW_PRESS;
}


bool GlfwInput::GamepadButtonHeldDown(int button, int gamepadID)
{
    if (gamepadID == _singlePlayerGamepadID)
        return !gamepadIDs.empty() ? GamepadButtonHeldDown(button, gamepadIDs[0]) : 0;
    
    GLFWgamepadstate state;
    glCall(bool found = glfwGetGamepadState(gamepadID, &state));
    if (!found)
        return false;
    return state.buttons[button]; // GLFW_PRESS converts to true and GLFW_RELEASE converts to false. There are no other button states.
}

bool GlfwInput::GamepadButtonWasHeldDown(int button, int gamepadID)
{
    if (gamepadID == _singlePlayerGamepadID)
        return !gamepadIDs.empty() ? GamepadButtonWasHeldDown(button, gamepadIDs[0]) : 0;
    
    if (!Tools::ContainsKey(lastGamepadStateByJoystick, (unsigned int)gamepadID))
        return false;

    return lastGamepadStateByJoystick.at(gamepadID).buttons[button];
}

// ------------------ get input (floats) -------------------

std::pair<float, float> GlfwInput::MouseScreenPosition()
{
    double xpos, ypos; // screen coordinates relative to the upper-left corner.
    auto window = OpenGlSetup::GetWindow();
    glCall(glfwGetCursorPos(window, &xpos, &ypos));
    return { (float)xpos, (float)ypos };
}

float GlfwInput::GamepadFloat(int axis, int gamepadID)
{
    if (gamepadID == _singlePlayerGamepadID)
        return !gamepadIDs.empty() ? GamepadFloat(axis, gamepadIDs[0]) : 0;

    GLFWgamepadstate state;
    glCall(bool found = glfwGetGamepadState(gamepadID, &state));
    if (!found)
        return 0;
    float rawActivation = state.axes[axis]; // the axis state is a number wintin [-1, 1]
    if (axis == GLFW_GAMEPAD_AXIS_LEFT_Y || axis == GLFW_GAMEPAD_AXIS_RIGHT_Y)
        rawActivation = -rawActivation; // y coordinates is upside down, ergo we flip it. 
    else if (axis == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER || axis == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER)
        rawActivation = 0.5f * rawActivation + 0.5f; // convert [-1, 1] into [0, 1]
    return std::abs(rawActivation) > noiseThreshold ? rawActivation : 0;
}


// ------------------ callbacks -------------------


void GlfwInput::_FocusChangeCallBack(GLFWwindow* window, int focused)
{
    if (focused == GLFW_FALSE)
        logger::print("glfw Lost focus");
    else if (focused == GLFW_TRUE)
        logger::print("glfw Gained focus");
}


void GlfwInput::_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // if you implement an event type, then you can invoke it here with (key, action, mods) as input
    // or perhaps listen for specific key, action, mods combination rather than receiving this as parameters
    // action can be 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT 

    if (key == GLFW_KEY_RIGHT_CONTROL)
        key = GLFW_KEY_LEFT_CONTROL; // we don't distinguish right and left ctrl, since ImGUI doesn't do so.
    actionByKey[key] = action;
}


void GlfwInput::_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    actionByMouseButton[button] = action;
}

// the docs say that pressed and direction are xOffset and yOffset. I guess that might hold for touchpad, but not for mouse.
void GlfwInput::_ScrollCallback(GLFWwindow* window, double pressed, double direction)
{
    scrollDirection = glm::clamp((float)direction, -1.0f, 1.0f);
}

void GlfwInput::_JoystickCallback(int gamepadID_, int event) 
{
    unsigned int gamepadID = (unsigned int)gamepadID_;

    // don't add non-gamepad joysticks. They are not supported.
    if (event == GLFW_CONNECTED)
    {
        glCall(bool isGamePad = glfwJoystickIsGamepad(gamepadID));
        if (!isGamePad)
            return;
    }

    if (event == GLFW_CONNECTED)
        gamepadIDs.push_back(gamepadID);
    else if (event == GLFW_DISCONNECTED)
        Tools::Remove(gamepadIDs, gamepadID);
    else
        Warning("Unrecognized eventType: ", event);

    InputKey::onGamepadConnectionChanged.Invoke(event == GLFW_CONNECTED, gamepadID);
}
