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
	inline static int GetWidth() { return WindowSize().first; } 
	inline static int GetHeight() { return WindowSize().second; } 
	inline static GLFWwindow* GetWindow() { if (window) return window; else RaiseError("GLFWwindow is nullptr"); } // returns ptr for convenience
	inline static bool WindowIsResized() { return WindowSize() != lastWindowSize; }

private:
	static bool openGLInitialized;
	static GLFWwindow* window;
	static std::pair<int, int> lastWindowSize;

	static std::pair<int, int> WindowSize();
};


