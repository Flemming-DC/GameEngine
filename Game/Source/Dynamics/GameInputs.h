#pragma once
#include "Engine.h"
#include "InputAction.h"

namespace GameInputs 
{
	InputAction<bool>& Jump();
	InputAction<float>& Fire();
	InputAction<glm::vec2>& Walk();
};