#include "EditorInputs.h"

using namespace Editor::EditorInputs;
using glm::vec2;


// scene editor - camera motion

static auto& zoom = InputAction<float>::Create()
	.AddKey(Key::FloatKey::mouseScrollVelocity)
	.AddKey(Key::FloatKey::rightStick_y);
static auto& directional = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::WASD)
	.AddKey(Key::VectorKey::Arrows)
	.AddKey(Key::VectorKey::leftStick);
static auto& dragCamera = InputAction<bool>::Create()
	.AddKey(Key::Mouse::right);

// scene editor - selection
static auto& select = InputAction<bool>::Create()
	.AddKey(Key::Mouse::left);
static auto& screenPosition = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::MouseEntireScreenPosition);
static auto& keepSelection = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::leftShift)
	.AddKey(Key::Keyboard::rightShift);
static auto& moveSelectionsNotCamera = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::leftShift)
	.AddKey(Key::Keyboard::rightShift);
/*
static auto& selectionMoveDirection = InputAction<vec2>::Create()
	.AddKey(Key::VectorKey::Arrows);
	//.AddKey(Key::VectorKey::WASD)
	//.AddConditionalKey(Key::Keyboard::leftShift); // add shift + WASD
*/
static auto& controlPosition = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_1); // evt. add shift condition
static auto& controlRotation = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_2); // evt. add shift condition
static auto& controlScale = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_3); // evt. add shift condition
static auto& controlUniformScale = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_4); // evt. add shift condition


// ------------------ getters -------------------

namespace Editor::EditorInputs
{
	InputAction<float>& Zoom() { return zoom; };
	vec2 MoveCamera() { return moveSelectionsNotCamera.IsPressed() ? vec2() : directional.State(); };
	InputAction<bool>& DragCamera() { return dragCamera; };

	// selection
	InputAction<bool>& Select() { return select; };
	InputAction<vec2>& ScreenPosition() { return screenPosition; };
	InputAction<bool>& KeepSelection() { return keepSelection; };
	vec2 SelectionMoveDirection() { return moveSelectionsNotCamera.IsPressed() ? directional.State() : vec2(); };
	//InputAction<vec2>& SelectionMoveDirection() { return selectionMoveDirection; };
	InputAction<bool>& ControlPosition() { return controlPosition; };
	InputAction<bool>& ControlRotation() { return controlRotation; };
	InputAction<bool>& ControlScale() { return controlScale; };
	InputAction<bool>& ControlUniformScale() { return controlUniformScale; };


	// EditorLoop
	bool ToggleRuntime() { return InputKey::BecomesPressed(Key::Keyboard::R); }
	bool Exit() { return InputKey::BecomesPressed(Key::Keyboard::exc); }
	bool Save() {  return InputKey::BecomesPressed(Key::Keyboard::S) 
						&& InputKey::IsPressed(Key::Keyboard::ctrl); }

	/*
	vec2 SelectionMoveDirection()
	{
		auto arrows = InputVectorizer::GetVectorInput(Key::VectorKey::Arrows);
		bool shiftPressed = InputKey::IsPressed(Key::Keyboard::leftShift);
		auto WASD = shiftPressed ? InputVectorizer::GetVectorInput(Key::VectorKey::WASD) : vec2();
		return glm::LargerThan(arrows, WASD) ? arrows : WASD;
	}
	*/
}



