#pragma once
#include "GlmTools.h"
#include "Key.h"


class InputVectorizer
{
	// axis_left, axis_right, dpad, WASD

public:
	static glm::vec2 GetVectorInput(Key::VectorKey key, int gamepadID = _singlePlayerGamepadID); // = findSinglePlayerGamepad

private:
	static glm::vec2 VectorFromFloats(Key::FloatKey xKey, Key::FloatKey yKey, int gamepadID);
	static glm::vec2 VectorFromBools(Key::Gamepad leftKey, Key::Gamepad rightKey, Key::Gamepad upKey, Key::Gamepad downKey, int gamepadID);
	static glm::vec2 VectorFromBools(Key::Keyboard leftKey, Key::Keyboard rightKey, Key::Keyboard upKey, Key::Keyboard downKey);
};

