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

static auto& controlPosition = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_1); 
static auto& controlRotation = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_2); 
static auto& controlScale = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_3); 
static auto& controlUniformScale = InputAction<bool>::Create()
	.AddKey(Key::Keyboard::_4); 


// ------------------ getters -------------------

namespace Editor::EditorInputs
{
	// --------- modifiers --------- //
	bool Spacebar()								{ return InputKey::IsPressed(Key::Keyboard::space); };
	bool Ctrl()									{ return InputKey::IsPressed(Key::Keyboard::ctrl); };
	bool Shift()								{ return InputKey::IsPressed(Key::Keyboard::leftShift) 
												      || InputKey::IsPressed(Key::Keyboard::rightShift); };

	// --------- actions --------- //
	InputAction<float>& Zoom()					{ return zoom; };
	InputAction<bool>& DragCamera()				{ return dragCamera; };
	vec2 MoveCamera()							{ return !Shift() && !Ctrl() ? directional.State() : vec2(); };

	// selection
	InputAction<bool>& Select()					{ return select; };
	InputAction<vec2>& ScreenPosition()			{ return screenPosition; };
	InputAction<bool>& KeepSelection()			{ return keepSelection; };
	vec2			   SelectionMoveDirection() { return Shift() && !Ctrl() ? directional.State() : vec2(); };
	InputAction<bool>& ControlPosition()		{ return controlPosition; }; // evt. add shift condition
	InputAction<bool>& ControlRotation()		{ return controlRotation; }; // evt. add shift condition
	InputAction<bool>& ControlScale()			{ return controlScale; }; // evt. add shift condition
	InputAction<bool>& ControlUniformScale()	{ return controlUniformScale; }; // evt. add shift condition

	// Creation / Deletion
	bool SaveStoredEntity()		{ return Ctrl() && InputKey::BecomesPressed(Key::Keyboard::enter); }
	bool CreateStoredEntity()	{ return Ctrl() && InputKey::BecomesPressed(Key::Keyboard::rightShift); }
	bool DeleteSelection()		{ return Ctrl() && InputKey::BecomesPressed(Key::Keyboard::_delete); }
	bool DublicateSelection()	{ return Ctrl() && InputKey::BecomesPressed(Key::Keyboard::D); }
	
	// hierachy
	bool FinishDragDrop()		{ return InputKey::BecomesReleased(Key::Mouse::left); } 
	bool Rename()				{ return InputKey::BecomesPressed(Key::Keyboard::F2); }

	// EditorLoop
	bool ToggleRuntime()		{ return Spacebar()  && InputKey::BecomesPressed(Key::Keyboard::R); }
	bool Exit()					{ return				InputKey::BecomesPressed(Key::Keyboard::exc); }
	bool Save()					{ return Ctrl()		 &&	InputKey::BecomesPressed(Key::Keyboard::S); }

}



