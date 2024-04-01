#include "EditorInputs.h"

using namespace Editor::EditorInputs;
using glm::vec2;


// scene editor - camera motion
auto& zoom = InputAction<float>::Create()
	.AddKey(Key::FloatKey::mouseScrollVelocity)
	.AddKey(Key::FloatKey::rightStick_y);
auto& moveCamera = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::WASD)
	.AddKey(Key::VectorKey::leftStick);

// scene editor - selection
auto& select = InputAction<bool>::Create()
	.AddKey(Key::Mouse::left);
auto& selectionPosition = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::mouseWorldPosition2D);
auto& keepSelection = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::leftShift)
	.AddKey(Key::Keyboard::rightShift);


// ------------------ getters -------------------

namespace Editor::EditorInputs
{
	InputAction<float>& Zoom() { return zoom; };
	InputAction<vec2>& MoveCamera() { return moveCamera; };
	InputAction<bool>& Select() { return select; };
	InputAction<vec2>& SelectionPosition() { return selectionPosition; };
	InputAction<bool>& KeepSelection() { return keepSelection; };
}



