#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "Key.h"

   

class KeyMap
{
public:
	static int ToGlfw(Key::Keyboard key);
	static int ToGlfw(Key::Mouse key);
	static int ToGlfw(Key::Gamepad key);
	static int ToGlfw(Key::FloatKey key);

	static ImGuiKey ToImGui(Key::Keyboard key);
	static ImGuiKey ToImGui(Key::Mouse key);
};






