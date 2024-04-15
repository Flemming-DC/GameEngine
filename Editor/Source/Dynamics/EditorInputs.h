#pragma once
#include "InputAction.h"
#include "Dynamic.h"


namespace Editor::EditorInputs
{
	using vec2 = glm::vec2;
	InputAction<float>& Zoom();
	InputAction<vec2>& MoveCamera();

	// selection
	InputAction<bool>& Select();
	InputAction<vec2>& ScreenPosition();
	InputAction<bool>& KeepSelection();
	InputAction<vec2>& SelectionMoveDirection();

	InputAction<bool>& ControlPosition();
	InputAction<bool>& ControlRotation();
	InputAction<bool>& ControlScale();


}