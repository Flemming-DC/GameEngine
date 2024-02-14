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
	static bool IsInitialized() { return isInitialized; }
private:
	static bool isInitialized;
};


