#pragma once
#include <utility>
#include "KeyMap.h"
#include "GlmTools.h"

class Input
{
public:
	static void Setup();
	static void LateUpdate();

	// Screen coordinates relative to the upper-left corner.
	// nb: that screen position coordinates are not comparable between ImGui / editor and glfw / game
	static glm::vec2 MouseScreenPosition();
	static glm::vec2 NormalizedMouseScreenPosition();
	static glm::vec3 MouseWorldPosition();
	static glm::vec2 MouseWorldPosition2D();
	static int GetScrollDirection();

	static bool IsPressed(Keyboard key);
	static bool IsReleased(Keyboard key);
	static bool IsHeldDown(Keyboard key);

	static bool IsPressed(Mouse key);
	static bool IsReleased(Mouse key);
	static bool IsHeldDown(Mouse key);


};

