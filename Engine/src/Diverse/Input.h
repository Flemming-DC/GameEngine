#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "KeyMap.h"


class Input
{
public:
	static void Setup(GLFWwindow* window_);
	static bool KeyHeldDown(Keyboard key);
	static bool GetScrollHeldDown();
	static int GetScrollDirection();
	static void Update();
	
private:
	static bool glwfScrollHeldDown;  // currently unused by engine
	static int glwfScrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll

	static void LogFocusChange(GLFWwindow* window, int focused);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double pressed, double direction);
	static ImGuiKey GetImGuiKey(int glfwKey);

};

