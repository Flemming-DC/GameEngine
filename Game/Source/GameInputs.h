#pragma once
#include "InputAction.h"
// don't import engine.h here

namespace GameInputs 
{
	InputAction<bool>& Jump();
	InputAction<float>& Fire();
	InputAction<glm::vec2>& Walk();
};