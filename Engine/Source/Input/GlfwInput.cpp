#include "GlfwInput.h"
#include "ErrorChecker.h"
#include "OpenGlSetup.h" 
#include "ListTools.h"
#include "logger.h"


int GlfwInput::scrollDirection = 0;
std::unordered_map<int, int> GlfwInput::actionByKey;
std::unordered_map<int, int> GlfwInput::actionByMouseButton;
std::vector<unsigned int> GlfwInput::joystick_ids;
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

    for (int jid = 0; jid < GLFW_JOYSTICK_LAST; jid++)
    {
        if (HasGamepad(jid))
            joystick_ids.push_back((unsigned int)jid);
    }
}

void GlfwInput::LateUpdate()
{
    // calculate lastGamepadStateByJoystick to be used in the next frame.
    lastGamepadStateByJoystick.clear();
    for (const auto& jid : joystick_ids)
    {
        GLFWgamepadstate state;
        glCall(bool found = glfwGetGamepadState(jid, &state));
        if (found)
            lastGamepadStateByJoystick[jid] = state;
    }
    

    // resetting input data
    scrollDirection = 0;
    actionByKey.clear();
    actionByMouseButton.clear();
}

bool GlfwInput::HasGamepad(int glfw_joystick_id)
{
    if (glfw_joystick_id == singlePlayerJoystick)
        return !joystick_ids.empty();

    // checks whether joystick is present and is gamepad. 
    // If we only use gamepads, then this in effect checks whether you have a gamepad or not.
    glCall(bool isGamePad = glfwJoystickIsGamepad(glfw_joystick_id));
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


bool GlfwInput::GamepadButtonHeldDown(int button, int glfw_joystick_id)
{
    if (glfw_joystick_id == singlePlayerJoystick)
        return !joystick_ids.empty() ? GamepadButtonHeldDown(button, joystick_ids[0]) : 0;
    
    GLFWgamepadstate state;
    glCall(bool found = glfwGetGamepadState(glfw_joystick_id, &state));
    if (!found)
        return false;
    return state.buttons[button]; // GLFW_PRESS converts to true and GLFW_RELEASE converts to false. There are no other button states.
}

bool GlfwInput::GamepadButtonWasHeldDown(int button, int glfw_joystick_id)
{
    if (glfw_joystick_id == singlePlayerJoystick)
        return !joystick_ids.empty() ? GamepadButtonWasHeldDown(button, joystick_ids[0]) : 0;
    
    if (!Tools::ContainsKey(lastGamepadStateByJoystick, (unsigned int)glfw_joystick_id))
        return false;

    return lastGamepadStateByJoystick.at(glfw_joystick_id).buttons[button];
}

// ------------------ get input (floats) -------------------

std::pair<float, float> GlfwInput::MouseScreenPosition()
{
    double xpos, ypos; // screen coordinates relative to the upper-left corner.
    auto window = OpenGlSetup::GetWindow();
    glCall(glfwGetCursorPos(window, &xpos, &ypos));
    return { xpos, ypos };
}

float GlfwInput::GamepadAxis(int axis, int glfw_joystick_id)
{
    if (glfw_joystick_id == singlePlayerJoystick)
        return !joystick_ids.empty() ? GamepadAxis(axis, joystick_ids[0]) : 0;

    GLFWgamepadstate state;
    glCall(bool found = glfwGetGamepadState(glfw_joystick_id, &state));
    if (!found)
        return 0;
    float rawActivation = state.axes[axis]; // the axis state is a number wintin [-1, 1]
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

void GlfwInput::_ScrollCallback(GLFWwindow* window, double pressed, double direction)
{
    scrollDirection = (int)direction;
}

void GlfwInput::_JoystickCallback(int joystick_id, int event) // jid = glfw_joystick_id ?
{
    glCall(bool isGamePad = glfwJoystickIsGamepad(joystick_id));
    if (!isGamePad)
        return;

    if (event == GLFW_CONNECTED)
        joystick_ids.push_back((unsigned int)joystick_id);
    else if (event == GLFW_DISCONNECTED)
    {

        bool found = Tools::Remove(joystick_ids, (unsigned int)joystick_id);
        P("joystick_id, found: ", joystick_id, found);
    }
}
