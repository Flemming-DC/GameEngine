#pragma once
#include <unordered_map>
#include <GL/glew.h> // load openGl function pointers
#include <GLFW/glfw3.h> // make openGl window


class GlfwInput
{
	friend class Input;

private:
	static int scrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll
	static std::unordered_map<int, int> actionByKey; // glwfAction: 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT
	static std::unordered_map<int, int> actionByMouseButton;

	static void Setup();
	static void LateUpdate();

	static bool KeyHeldDown(int key);
	static bool MouseButtonHeldDown(int button);
	static std::pair<double, double> MouseScreenPosition();

// private even to friend class:
	static void _FocusChangeCallBack(GLFWwindow* window, int focused);
	static void _KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void _ScrollCallback(GLFWwindow* window, double pressed, double direction);
	static void _MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


};

