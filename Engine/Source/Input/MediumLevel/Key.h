#pragma once



const int _singlePlayerGamepadID = -1; // private to the input system

namespace Key
{
	enum class Keyboard
	{
		exc, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, printScreen, ScrollLock, pause,
		_1, _2, _3, _4, _5, _6, _7, _8, _9, _0, plus, apostrophe, back, insert, home, pageup, num, numpad_div, numpad_mul, numpad_minus,
		tab, Q, W, E, R, T, Y, U, I, O, P, enter, _delete, end, pagedown, numpad_home, numpad_7, numpad_8, numpad_9, numpad_plus,
		capslock, A, S, D, F, G, H, J, K, L, pling, numpad_4, numpad_5, numpad_6, 
		leftShift, Z, X, C, V, B, N, M, comma, period, dash, rightShift, upArrow, numpad_1, numpad_2, numpad_3, numpad_enter,
		ctrl, alt, space, altgr, leftArrow, downArrow, rightArrow, numpad_0, 

		// removed keys: hat, plus, lessThan, numpad_del, , half, 
	};

	// by starting with a number over max value in previous device, then the enums are unique betweens devices
	enum class Mouse
	{
		left, middle, right,
	};

	enum class Gamepad
	{
		// buttons
		A, B, X, Y, leftBumper, rightBumper, back, start, guide, leftStickPress, rightStickPress, dpad_up, dpad_right, dpad_down, dpad_left,
		// left_thumb, right_thumb has been renamed to left_stick_press, right_stick_press

		// axes
		//axis_left_x, axis_left_y, axis_right_x, axis_right_y, axis_left_trigger, axis_right_trigger,

		// evt. make aliases (cross, circle, square, triangle) for (A, B, X, Y)
		// evt. combine joystick-axis and dpad into vectors
	};

	enum class FloatKey
	{
		leftStick_x, leftStick_y, rightStick_x, rightStick_y, leftTrigger, rightTrigger, mouseScrollVelocity
	};


	enum class VectorKey
	{
		leftStick, rightStick, dpad, WASD, Arrows,
		MouseEntireScreenPosition, MouseGameScreenPosition, mouseWorldPosition2D,
	};

}

//MouseEntireScreenPosition, MouseGameScreenPosition