#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Input
{
public:
	static void MakeEntities(GLFWwindow* window_);
	static bool KeyHeldDown(int key);
	inline static bool GetScrollHeldDown() { return scrollHeldDown; }
	inline static int GetScrollDirection() { return scrollDirection; }
	static void Update();

private:
	static bool scrollHeldDown;  // currently unused by engine
	static int scrollDirection; // 1 = forward, -1 = backwards, 0 = no scroll

	static void LogFocusChange(GLFWwindow* window, int focused);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double pressed, double direction);

};

