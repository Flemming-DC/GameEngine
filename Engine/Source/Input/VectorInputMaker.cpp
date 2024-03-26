#include "VectorInputMaker.h"
#include "Input.h"
#include "KeyMap.h"


glm::vec2 VectorInputMaker::GetVectorInput(VectorInput key, int gamepadID)
{
    switch (key)
    {
	case VectorInput::joystickLeft: return VectorFromFloats(Gamepad::axis_left_x, Gamepad::axis_left_y, gamepadID);
	case VectorInput::joystickRight: return VectorFromFloats(Gamepad::axis_right_x, Gamepad::axis_right_y, gamepadID);
	case VectorInput::dpad: return VectorFromBools(Gamepad::dpad_left, Gamepad::dpad_right, Gamepad::dpad_up, Gamepad::dpad_down, gamepadID);
	case VectorInput::WASD: return VectorFromBools(Keyboard::A, Keyboard::D, Keyboard::W, Keyboard::S);
	case VectorInput::Arrows: return VectorFromBools(Keyboard::leftArrow, Keyboard::rightArrow, Keyboard::upArrow, Keyboard::downArrow);
	default:
        RaiseError("Missing key", key);
		return {0, 0}; // dummy return value
    }
}




// -------------------- private helper functions --------------------


glm::vec2 VectorInputMaker::VectorFromFloats(Gamepad xKey, Gamepad yKey, int gamepadID)
{
	float x = Input::GamepadAxis(xKey, gamepadID);
	float y = Input::GamepadAxis(yKey, gamepadID);
	return { x / glm::sqrt(2), y / glm::sqrt(2) }; // dividing by sqrt(2) normalizes the vector, so that x=1, y=1 yields magnitude=1.
}

glm::vec2 VectorInputMaker::VectorFromBools(Gamepad leftKey, Gamepad rightKey, Gamepad upKey, Gamepad downKey, int gamepadID)
{
	int left  = Input::IsHeldDown(leftKey,  gamepadID);
	int right = Input::IsHeldDown(rightKey, gamepadID);
	int up	  = Input::IsHeldDown(upKey,	gamepadID);
	int down  = Input::IsHeldDown(downKey,  gamepadID);
	return { right - left, up - down };
}

glm::vec2 VectorInputMaker::VectorFromBools(Keyboard leftKey, Keyboard rightKey, Keyboard upKey, Keyboard downKey)
{
	int left = Input::IsHeldDown(leftKey);
	int right = Input::IsHeldDown(rightKey);
	int up = Input::IsHeldDown(upKey);
	int down = Input::IsHeldDown(downKey);
	return { right - left, up - down };
}
