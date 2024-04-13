#include "InputVectorizer.h"
#include "InputKey.h"
#include "GlfwInput.h" // just to get the noise threshold

using namespace Key;
static float const noiseNormThreshold = 2 * GlfwInput::noiseThreshold * GlfwInput::noiseThreshold;

glm::vec2 InputVectorizer::GetVectorInput(VectorKey key, int gamepadID)
{
    switch (key)
    {
	case VectorKey::leftStick: return VectorFromFloats(FloatKey::leftStick_x, FloatKey::leftStick_y, gamepadID);
	case VectorKey::rightStick: return VectorFromFloats(FloatKey::rightStick_x, FloatKey::rightStick_y, gamepadID);
	case VectorKey::dpad: return VectorFromBools(Gamepad::dpad_left, Gamepad::dpad_right, Gamepad::dpad_up, Gamepad::dpad_down, gamepadID);
	case VectorKey::WASD: return VectorFromBools(Keyboard::A, Keyboard::D, Keyboard::W, Keyboard::S);
	case VectorKey::Arrows: return VectorFromBools(Keyboard::leftArrow, Keyboard::rightArrow, Keyboard::upArrow, Keyboard::downArrow);
	case VectorKey::MouseEntireScreenPosition: return InputKey::MouseEntireScreenPosition();
	case VectorKey::MouseGameScreenPosition: return InputKey::MouseGameScreenPosition();
	case VectorKey::mouseWorldPosition2D: return InputKey::MouseWorldPosition2D();
	default:
        RaiseError("Missing key", key);
		return {0, 0}; // dummy return value
    }
}




// -------------------- private helper functions --------------------


glm::vec2 InputVectorizer::VectorFromFloats(FloatKey xKey, FloatKey yKey, int gamepadID)
{
	float x = InputKey::GetFloat(xKey, gamepadID);
	float y = InputKey::GetFloat(yKey, gamepadID);
	glm::vec2 vec = { x, y };
	if (glm::dot(vec, vec) < noiseNormThreshold)
		return { 0, 0 }; // return early to avoid division by zero
	float maximumNorm = glm::max(std::abs(vec.x), std::abs(vec.y));
	vec = maximumNorm / glm::length(vec) * vec; // convert maximumNorm into the standard euclidean norm
	return vec;
}

glm::vec2 InputVectorizer::VectorFromBools(Gamepad leftKey, Gamepad rightKey, Gamepad upKey, Gamepad downKey, int gamepadID)
{
	float left  = InputKey::IsPressed(leftKey,  gamepadID);
	float right = InputKey::IsPressed(rightKey, gamepadID);
	float up	  = InputKey::IsPressed(upKey,	gamepadID);
	float down  = InputKey::IsPressed(downKey,  gamepadID);
	glm::vec2 vec = { right - left, up - down };
	if (glm::dot(vec, vec) > 1)
		vec = glm::normalize(vec); // normalize diagonal vectors
	return vec;
}

glm::vec2 InputVectorizer::VectorFromBools(Keyboard leftKey, Keyboard rightKey, Keyboard upKey, Keyboard downKey)
{
	float left = InputKey::IsPressed(leftKey);
	float right = InputKey::IsPressed(rightKey);
	float up = InputKey::IsPressed(upKey);
	float down = InputKey::IsPressed(downKey);
	glm::vec2 vec = { right - left, up - down };
	if (glm::dot(vec, vec) > 1)
		vec = glm::normalize(vec); // normalize diagonal vectors
	return vec;
}
