#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"

enum class Keyboard 
{
	exc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, printScreen, keyboradScroll, pause,
	half, _1, _2, _3, _4, _5, _6, _7, _8, _9, _0, plus, apostrophe, back, insert, home, pageup, num, numpad_div, numpad_mul, numpad_minus,
	tab, Q, W, E, R, T, Y, U, I, O, P, hat, enter, _delete, end, pagedown, numpad_home, numpad_7, numpad_8, numpad_9, numpad_plus,
	capslock, A, S, D, F, G, H, J, K, L, pling, numpad_4, numpad_5, numpad_6,
	leftShift, lessThan, Z, X, C, V, B, N, M, comma, period, dash, rightShift, upArrow, numpad_1, numpad_2, numpad_3, numpad_enter,
	ctrl, alt, space, altgr, leftArrow, downArrow, rightArrow, numpad_0, numpad_del,
};

enum class Mouse
{
	left, middle, right,
};

enum class CursorMode
{
	normal, hidden, locked,
};

// enum class Gamepad

class KeyMap
{
public:
	static int ToGlfw(Keyboard key);
	static int ToGlfw(Mouse key);

	static ImGuiKey ToImGui(Keyboard key);
	static ImGuiKey ToImGui(Mouse key);
};






