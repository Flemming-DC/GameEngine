#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>



class Input
{
public:
	static void Setup(GLFWwindow* window_);
	static bool GetKey(int key);

private:
	static void LogFocusChange(GLFWwindow* window, int focused);
	static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

