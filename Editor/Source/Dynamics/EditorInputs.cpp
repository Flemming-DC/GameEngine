#include "EditorInputs.h"

using namespace Editor::EditorInputs;
using glm::vec2;


// scene editor - camera motion

static auto& zoom = InputAction<float>::Create()
	.AddKey(Key::FloatKey::mouseScrollVelocity)
	.AddKey(Key::FloatKey::rightStick_y);
static auto& moveCamera = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::WASD)
	.AddKey(Key::VectorKey::leftStick);

// scene editor - selection
static auto& select = InputAction<bool>::Create()
	.AddKey(Key::Mouse::left);
static auto& screenPosition = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::MouseEntireScreenPosition);
static auto& keepSelection = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::leftShift)
	.AddKey(Key::Keyboard::rightShift);
static auto& selectionMoveDirection = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::Arrows); // add shift + WASD
static auto& controlPosition = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_1); // evt. add shift condition
static auto& controlRotation = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_2); // evt. add shift condition
static auto& controlScale = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_3); // evt. add shift condition


// ------------------ getters -------------------

namespace Editor::EditorInputs
{
	InputAction<float>& Zoom() { return zoom; };
	InputAction<vec2>& MoveCamera() { return moveCamera; };
	InputAction<bool>& Select() { return select; };
	InputAction<vec2>& ScreenPosition() { return screenPosition; };
	InputAction<bool>& KeepSelection() { return keepSelection; };
	InputAction<vec2>& SelectionMoveDirection() { return selectionMoveDirection; };
	InputAction<bool>& ControlPosition() { return controlPosition; };
	InputAction<bool>& ControlRotation() { return controlRotation; };
	InputAction<bool>& ControlScale() { return controlScale; };




}



