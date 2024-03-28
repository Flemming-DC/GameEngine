#include "GameInputs.h"

InputAction<bool>& GameInputs::jump = InputAction<bool>(0);
InputAction<float>& GameInputs::fire = InputAction<float>(0);
InputAction<glm::vec2>& GameInputs::walk = InputAction<glm::vec2>(0);


void GameInputs::OnGameStart()
{

	jump = InputAction<bool>::Create()
		.AddKey(Key::Keyboard::space)
		.AddKey(Key::Mouse::right)
		.AddKey(Key::Gamepad::A);

	fire = InputAction<float>::Create()
		.AddKey(Key::FloatKey::mouseScrollDirection)
		.AddKey(Key::FloatKey::leftTrigger);

	walk = InputAction<glm::vec2>::Create()
		.AddKey(Key::VectorKey::WASD)
		.AddKey(Key::VectorKey::leftStick);

	auto& dum = InputAction<glm::vec2>::Create()
		.AddKey(Key::VectorKey::Arrows)
		.AddKey(Key::VectorKey::rightStick);
	dum.Destroy();
}

void GameInputs::OnGameEnd()
{
	jump.Destroy();
	fire.Destroy();
	walk.Destroy();
}