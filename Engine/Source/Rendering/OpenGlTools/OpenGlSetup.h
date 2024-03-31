#pragma once
#include "OpenGlExternal.h"
#include "OpenGlError.h"


class OpenGlSetup
{
public:
	static void Setup();
	static void Shutdown();
	static bool Update();
	static bool Initialized() { return openGLInitialized; }
	inline static int GetWidth() { return width; }
	inline static int GetHeight() { return height; }
	inline static GLFWwindow* GetWindow() { if (window) return window; else RaiseError("GLFWwindow is nullptr"); } // returns ptr for convenience

private:
	static bool openGLInitialized;
	static int width;
	static int height;
	static GLFWwindow* window;
};


