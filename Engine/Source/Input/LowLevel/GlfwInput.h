#pragma once
#include <unordered_map>
#include "OpenGlExternal.h"


class GlfwInput
{
	friend class Input;
public:
	inline const static float noiseThreshold = 0.1f; // the activation level of gamepad axis that seperates background noise from genuine input.
private:
	inline const static int findSinglePlayerGamepad = -1; 
	static int scrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll
	static std::unordered_map<int, int> actionByKey; // glwfAction: 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT
	static std::unordered_map<int, int> actionByMouseButton;
	static std::unordered_map<unsigned int, GLFWgamepadstate> lastGamepadStateByJoystick;
	static std::vector<unsigned int> gamepadIDs;

	static void Setup();
	static void LateUpdate();

	static bool KeyHeldDown(int key);
	static bool MouseButtonHeldDown(int button);
	static bool GamepadButtonHeldDown(int button, int gamepadID = findSinglePlayerGamepad); // fx GLFW_JOYSTICK_1
	static bool GamepadButtonWasHeldDown(int button, int gamepadID = findSinglePlayerGamepad); // fx GLFW_JOYSTICK_1

	static std::pair<float, float> MouseScreenPosition();
	static float GamepadFloat(int axis, int gamepadID = findSinglePlayerGamepad);
	static bool HasGamepad(int gamepadID = findSinglePlayerGamepad);

// private even to friend class:
	static void _FocusChangeCallBack(GLFWwindow* window, int focused);
	static void _KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _ScrollCallback(GLFWwindow* window, double pressed, double direction);
	static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _JoystickCallback(int jid, int event); // onJoystickConnected and onJoystickDisConnected 

};

