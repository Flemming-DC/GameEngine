#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "KeyMap.h"


class Input
{
public:
	static void Setup();
	static void Update();

	static bool KeyPressed(Keyboard key);
	static bool KeyReleased(Keyboard key);
	static bool KeyHeldDown(Keyboard key);
	static bool GetScrollHeldDown();
	static int GetScrollDirection();
	
private:
	static bool glwfScrollHeldDown;  // currently unused by engine
	static int glwfScrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll

	static void GlfwLogFocusChange(GLFWwindow* window, int focused);
	static void GlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GlfwScrollCallback(GLFWwindow* window, double pressed, double direction);

};

