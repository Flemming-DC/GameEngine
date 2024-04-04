#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "Key.h"
#include <string>

   

class KeyMap
{
public:
	static int ToGlfw(Key::Keyboard key);
	static int ToGlfw(Key::Mouse key);
	static int ToGlfw(Key::Gamepad key);
	static int ToGlfw(Key::FloatKey key);

	static ImGuiKey ToImGui(Key::Keyboard key);
	static ImGuiKey ToImGui(Key::Mouse key);
	static ImGuiKey ToImGui(Key::Gamepad key);
	static ImGuiKey ToImGui(Key::FloatKey key);

};



namespace logger
{
	std::string to_string(Key::Keyboard key);
	std::string to_string(Key::Mouse key);
	std::string to_string(Key::Gamepad key);
	std::string to_string(Key::FloatKey key);
	std::string to_string(Key::VectorKey key);
}



