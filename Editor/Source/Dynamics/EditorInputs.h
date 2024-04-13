#pragma once
#include "InputAction.h"
#include "Dynamic.h"


namespace Editor::EditorInputs
{
	using vec2 = glm::vec2;
	InputAction<float>& Zoom();
	InputAction<vec2>& MoveCamera();
	InputAction<bool>& Select();
	InputAction<vec2>& ScreenPosition();
	InputAction<bool>& KeepSelection();


}