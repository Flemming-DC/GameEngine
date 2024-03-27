#pragma once
#include <utility>
#include "InputEnums.h"
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

	static float GetFloat(FloatKey axis, int gamepadID = findSinglePlayerGamepad);
	static bool HasGamepad(int gamepadID = findSinglePlayerGamepad);
	static std::vector<unsigned int> GamepadIDs();
	

	// -------------- IsHeldDown, IsPressed, IsReleased --------------
	
	static bool IsPressed(Keyboard key);
	static bool IsReleased(Keyboard key);
	static bool IsHeldDown(Keyboard key);

	static bool IsPressed(Mouse key);
	static bool IsReleased(Mouse key);
	static bool IsHeldDown(Mouse key);
	
	static bool IsPressed(Gamepad key, int gamepadID = findSinglePlayerGamepad);
	static bool IsReleased(Gamepad key, int gamepadID = findSinglePlayerGamepad);
	static bool IsHeldDown(Gamepad key, int gamepadID = findSinglePlayerGamepad);
private:
	static const unsigned int findSinglePlayerGamepad; // nb: this alias must match the corrosponding one in GlfwInput

	static int GetScrollDirection();
};

