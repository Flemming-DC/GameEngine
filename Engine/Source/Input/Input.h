#pragma once
#include <utility>
#include "KeyMap.h"
#include "GlmTools.h"

class Input
{
public:
	static void Setup();
	static void LateUpdate();

	// -------------- special input --------------
	
	// Screen coordinates relative to the upper-left corner.
	// nb: that screen position coordinates are not comparable between ImGui / editor and glfw / game
	static glm::vec2 MouseScreenPosition();
	static glm::vec2 NormalizedMouseScreenPosition();
	static glm::vec3 MouseWorldPosition();
	static glm::vec2 MouseWorldPosition2D();
	static int GetScrollDirection();

	static float GamepadAxis(Gamepad axis, int glfw_joystick_id = singlePlayerJoystick);
	static bool HasGamepad(int glfw_joystick_id = singlePlayerJoystick);

	// -------------- IsHeldDown, IsPressed, IsReleased --------------
	
	static bool IsPressed(Keyboard key);
	static bool IsReleased(Keyboard key);
	static bool IsHeldDown(Keyboard key);

	static bool IsPressed(Mouse key);
	static bool IsReleased(Mouse key);
	static bool IsHeldDown(Mouse key);
	
	static bool IsPressed(Gamepad key, unsigned int joystick_id = singlePlayerJoystick);
	static bool IsReleased(Gamepad key, unsigned int joystick_id = singlePlayerJoystick);
	static bool IsHeldDown(Gamepad key, unsigned int joystick_id = singlePlayerJoystick);
	
private:
	inline static const unsigned int singlePlayerJoystick = -1; // nb: this alias must match the corrosponding one in GlfwInput
};

