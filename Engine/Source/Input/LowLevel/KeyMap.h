#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "InputEnums.h"

   


class KeyMap
{
public:
	static int ToGlfw(Keyboard key);
	static int ToGlfw(Mouse key);
	static int ToGlfw(Gamepad key);
	static int ToGlfw(FloatKey key);

	static ImGuiKey ToImGui(Keyboard key);
	static ImGuiKey ToImGui(Mouse key);
};






