#include "InputVectorizer.h"
#include "Input.h"
#include "InputEnums.h"
#include "GlfwInput.h" // just to get the noise threshold

static float const noiseNormThreshold = 2 * GlfwInput::noiseThreshold * GlfwInput::noiseThreshold;

glm::vec2 InputVectorizer::GetVectorInput(InputVector key, int gamepadID)
{
    switch (key)
    {
	case InputVector::joystickLeft: return VectorFromFloats(Gamepad::axis_left_x, Gamepad::axis_left_y, gamepadID);
	case InputVector::joystickRight: return VectorFromFloats(Gamepad::axis_right_x, Gamepad::axis_right_y, gamepadID);
	case InputVector::dpad: return VectorFromBools(Gamepad::dpad_left, Gamepad::dpad_right, Gamepad::dpad_up, Gamepad::dpad_down, gamepadID);
	case InputVector::WASD: return VectorFromBools(Keyboard::A, Keyboard::D, Keyboard::W, Keyboard::S);
	case InputVector::Arrows: return VectorFromBools(Keyboard::leftArrow, Keyboard::rightArrow, Keyboard::upArrow, Keyboard::downArrow);
	default:
        RaiseError("Missing key", key);
		return {0, 0}; // dummy return value
    }
}




// -------------------- private helper functions --------------------


glm::vec2 InputVectorizer::VectorFromFloats(Gamepad xKey, Gamepad yKey, int gamepadID)
{
	float x = Input::GamepadFloat(xKey, gamepadID);
	float y = Input::GamepadFloat(yKey, gamepadID);
	glm::vec2 vec = { x, y };
	if (glm::dot(vec, vec) < noiseNormThreshold)
		return { 0, 0 }; // return early to avoid division by zero
	float maximumNorm = glm::max(std::abs(vec.x), std::abs(vec.y));
	vec = maximumNorm / glm::length(vec) * vec; // convert maximumNorm into the standard euclidean norm
	return vec;
}

glm::vec2 InputVectorizer::VectorFromBools(Gamepad leftKey, Gamepad rightKey, Gamepad upKey, Gamepad downKey, int gamepadID)
{
	float left  = Input::IsHeldDown(leftKey,  gamepadID);
	float right = Input::IsHeldDown(rightKey, gamepadID);
	float up	  = Input::IsHeldDown(upKey,	gamepadID);
	float down  = Input::IsHeldDown(downKey,  gamepadID);
	glm::vec2 vec = { right - left, up - down };
	if (glm::dot(vec, vec) > 1)
		vec = glm::normalize(vec); // normalize diagonal vectors
	return vec;
}

glm::vec2 InputVectorizer::VectorFromBools(Keyboard leftKey, Keyboard rightKey, Keyboard upKey, Keyboard downKey)
{
	float left = Input::IsHeldDown(leftKey);
	float right = Input::IsHeldDown(rightKey);
	float up = Input::IsHeldDown(upKey);
	float down = Input::IsHeldDown(downKey);
	glm::vec2 vec = { right - left, up - down };
	if (glm::dot(vec, vec) > 1)
		vec = glm::normalize(vec); // normalize diagonal vectors
	return vec;
}
