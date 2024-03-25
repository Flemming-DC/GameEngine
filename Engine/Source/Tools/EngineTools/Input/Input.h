#pragma once
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "KeyMap.h"


class Input
{
public:
	static void Setup();
	static void LateUpdate();

	static bool KeyPressed(Keyboard key);
	static bool KeyReleased(Keyboard key);
	static bool KeyHeldDown(Keyboard key);
	static bool GetScrollHeldDown();
	static int GetScrollDirection();
	
private:
	enum class glwfAction { pressed, released, heldDownLong, null }; // evt. add heldDown and make it into a general action class
	static bool glwfScrollHeldDown;  // currently unused by engine
	static int glwfScrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll
	static std::unordered_map<int, glwfAction> glwfActionByKey;

	static void GlfwLogFocusChange(GLFWwindow* window, int focused);
	static void GlfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GlfwScrollCallback(GLFWwindow* window, double pressed, double direction);


};

