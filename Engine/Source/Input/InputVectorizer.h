#pragma once
#include "GlmTools.h"
#include "InputEnums.h"


class InputVectorizer
{
	// axis_left, axis_right, dpad, WASD

public:
	static glm::vec2 GetVectorInput(InputVector key, int gamepadID = -1); // = findSinglePlayerGamepad

private:
	static glm::vec2 VectorFromFloats(Gamepad xKey, Gamepad yKey, int gamepadID);
	static glm::vec2 VectorFromBools(Gamepad leftKey, Gamepad rightKey, Gamepad upKey, Gamepad downKey, int gamepadID);
	static glm::vec2 VectorFromBools(Keyboard leftKey, Keyboard rightKey, Keyboard upKey, Keyboard downKey);
};

