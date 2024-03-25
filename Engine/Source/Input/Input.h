#pragma once
#include <utility>
#include "KeyMap.h"
#include "GlmTools.h"

class Input
{
public:
	static void Setup();
	static void LateUpdate();

	static int GetScrollDirection();
	static std::pair<double, double> MouseScreenPosition();
	static glm::vec3 MouseWorldPosition();

	static bool IsPressed(Keyboard key);
	static bool IsReleased(Keyboard key);
	static bool IsHeldDown(Keyboard key);

	static bool IsPressed(Mouse key);
	static bool IsReleased(Mouse key);
	static bool IsHeldDown(Mouse key);


};

