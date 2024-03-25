#pragma once
#include <unordered_map>
#include <GL/glew.h> // load openGl function pointers
#include <GLFW/glfw3.h> // make openGl window


class GlfwInput
{
	friend class Input;

private:
	static bool scrollHeldDown;  // currently unused by engine
	static int scrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll
	static std::unordered_map<int, int> actionByKey; // glwfAction: 0 = GLFW_RELEASE, 1 = GLFW_PRESS, 2 = GLFW_REPEAT

	static void Setup();
	static void LateUpdate();
	static void logFocusChange(GLFWwindow* window, int focused);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double pressed, double direction);
	static bool KeyHeldDown(int key);


};

