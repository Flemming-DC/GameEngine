#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGlError.h"


class OpenGlSetup
{
public:
	static void Setup();
	static void Shutdown();
	static bool NewFrame();
	static void EndFrame();
	static bool Initialized() { return openGLInitialized; }
	inline static int GetWidth() { return width; }
	inline static int GetHeight() { return height; }
	inline static GLFWwindow* GetWindow() { return window; }
	static void Exit();

private:
	static bool openGLInitialized;
	//static ImGuiIO io;
	static int width;
	static int height;
	static GLFWwindow* window;
};


