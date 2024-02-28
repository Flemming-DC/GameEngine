#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Initializer
{
public:
	static void Setup();
	static void Shutdown();
	static bool NewFrame();
	static void EndFrame();
	static bool OpenGLInitialized() { return openGLInitialized; }
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


