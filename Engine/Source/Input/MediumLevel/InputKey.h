#pragma once
#include <utility>
#include "Key.h"
#include "GlmTools.h"
#include "Event.h"

class InputKey
{
public:
	// Event<bool hasBecomeConnected, unsigned int gamepadID>
	static Event<bool, unsigned int> onGamepadConnectionChanged;


	// -------------- special input --------------
	
	// Screen coordinates relative to the upper-left corner.
	// nb: that screen position coordinates are not comparable between ImGui / editor and glfw / game
	static glm::vec2 MouseScreenPosition();
	static glm::vec2 NormalizedMouseScreenPosition();
	static glm::vec3 MouseWorldPosition();
	static glm::vec2 MouseWorldPosition2D();

	static float GetFloat(Key::FloatKey axis, int gamepadID = _singlePlayerGamepadID);
	static bool HasGamepad(int gamepadID = _singlePlayerGamepadID);
	static std::vector<unsigned int> GamepadIDs();
	

	// -------------- IsPressed, BecomesPressed, BecomesReleased --------------
	
	static bool BecomesPressed(Key::Keyboard key);
	static bool BecomesReleased(Key::Keyboard key);
	static bool IsPressed(Key::Keyboard key);

	static bool BecomesPressed(Key::Mouse key);
	static bool BecomesReleased(Key::Mouse key);
	static bool IsPressed(Key::Mouse key);
	
	static bool BecomesPressed(Key::Gamepad key, int gamepadID = _singlePlayerGamepadID);
	static bool BecomesReleased(Key::Gamepad key, int gamepadID = _singlePlayerGamepadID);
	static bool IsPressed(Key::Gamepad key, int gamepadID = _singlePlayerGamepadID);
private:

	static float GetScrollDirection();
};

