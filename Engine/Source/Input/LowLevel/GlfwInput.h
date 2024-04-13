#pragma once
#include <unordered_map>
#include "OpenGlExternal.h"
#include "Key.h" // we are only interested in _singlePlayerGamepadID 

class GlfwInput
{
	friend class InputKey;
public:
	inline const static float noiseThreshold = 0.1f; // the activation level of gamepad axis that seperates background noise from genuine input.
	
	static void Setup();
	static void LateUpdate();

private:
	static float scrollDelta; // normalized so that 1 = forward, -1 = backwards, 0 = no scroll. 
	static std::unordered_map<int, int> actionByKey; // glwfAction: 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT
	static std::unordered_map<int, int> actionByMouseButton;
	static std::unordered_map<unsigned int, GLFWgamepadstate> lastGamepadStateByJoystick;
	static std::vector<unsigned int> gamepadIDs;

	static bool KeyHeldDown(int key);
	static bool MouseButtonHeldDown(int button);
	static bool GamepadButtonHeldDown(int button, int gamepadID = _singlePlayerGamepadID); // fx GLFW_JOYSTICK_1
	static bool GamepadButtonWasHeldDown(int button, int gamepadID = _singlePlayerGamepadID); // fx GLFW_JOYSTICK_1

	static std::pair<float, float> MouseScreenPosition(bool relativeToWindow);
	static float GamepadFloat(int axis, int gamepadID = _singlePlayerGamepadID);
	static bool HasGamepad(int gamepadID = _singlePlayerGamepadID);

// private even to friend class:
	static void _FocusChangeCallBack(GLFWwindow* window, int focused);
	static void _KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _ScrollCallback(GLFWwindow* window, double pressed, double direction);
	static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _JoystickCallback(int jid, int event); // onJoystickConnected and onJoystickDisConnected 

};

