#pragma once
#include "InputAction.h"
#include "Dynamic.h"


namespace Editor::EditorInputs
{
	using vec2 = glm::vec2;
	InputAction<float>& Zoom();
	vec2 MoveCamera();
	InputAction<bool>& DragCamera();

	// selection
	InputAction<bool>& Select();
	InputAction<vec2>& ScreenPosition();
	InputAction<bool>& KeepSelection();
	vec2 SelectionMoveDirection();

	InputAction<bool>& ControlPosition();
	InputAction<bool>& ControlRotation();
	InputAction<bool>& ControlScale();


}