#pragma once
#include <utility>
#include "Key.h"
#include "GlmTools.h"
#include "Event.h"

class Input
{
public:
	// Event<bool hasBecomeConnected, unsigned int gamepadID>
	static Event<bool, unsigned int> onGamepadConnectionChanged;

	static void Setup();
	static void LateUpdate();

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
	

	// -------------- IsHeldDown, IsPressed, IsReleased --------------
	
	static bool IsPressed(Key::Keyboard key);
	static bool IsReleased(Key::Keyboard key);
	static bool IsHeldDown(Key::Keyboard key);

	static bool IsPressed(Key::Mouse key);
	static bool IsReleased(Key::Mouse key);
	static bool IsHeldDown(Key::Mouse key);
	
	static bool IsPressed(Key::Gamepad key, int gamepadID = _singlePlayerGamepadID);
	static bool IsReleased(Key::Gamepad key, int gamepadID = _singlePlayerGamepadID);
	static bool IsHeldDown(Key::Gamepad key, int gamepadID = _singlePlayerGamepadID);
private:

	static int GetScrollDirection();
};

