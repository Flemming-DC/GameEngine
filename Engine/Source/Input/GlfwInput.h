#pragma once
#include <unordered_map>
#include <GL/glew.h> // load openGl function pointers
#include <GLFW/glfw3.h> // make openGl window


class GlfwInput
{
	friend class Input;

public:
//private:
	static int singlePlayerJoystick;
	static int scrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll
	static std::unordered_map<int, int> actionByKey; // glwfAction: 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT
	static std::unordered_map<int, int> actionByMouseButton;
	static std::unordered_map<int, int> actionByGamepadButton;
	static std::vector<unsigned int> joystick_ids;

	static void Setup();
	static void LateUpdate();

	static bool KeyHeldDown(int key);
	static bool MouseButtonHeldDown(int button);
	static bool GamepadButtonHeldDown(int button, int glfw_joystick_id = -1); // fx -1 = singlePlayerJoystick

	static std::pair<float, float> MouseScreenPosition();
	static float GamepadAxis(int axis, int glfw_joystick_id = -1);
	static bool HasGamepad(int glfw_joystick_id = -1);

// private even to friend class:
	static void _FocusChangeCallBack(GLFWwindow* window, int focused);
	static void _KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _ScrollCallback(GLFWwindow* window, double pressed, double direction);
	static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void _JoystickCallback(int jid, int event);

};

