#include "EditorInputs.h"
using namespace Editor;


InputAction<float>* EditorInputs::zoom = nullptr;
InputAction<glm::vec2>* EditorInputs::moveCamera = nullptr;

void EditorInputs::OnGameStart()
{
	zoom = &InputAction<float>::Create()
		.AddKey(Key::FloatKey::mouseScrollVelocity)
		.AddKey(Key::FloatKey::rightStick_y);

	moveCamera = &InputAction<glm::vec2>::Create()
		.AddKey(Key::VectorKey::WASD)
		.AddKey(Key::VectorKey::leftStick);
}