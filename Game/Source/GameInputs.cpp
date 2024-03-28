#include "GameInputs.h"

InputAction<bool>& GameInputs::jump = InputAction<bool>();
InputAction<float>& GameInputs::fire = InputAction<float>();
InputAction<glm::vec2>& GameInputs::walk = InputAction<glm::vec2>();


void GameInputs::OnGameStart()
{
	P("jump");
	jump = InputAction<bool>::Create()
		.AddKey(Key::Keyboard::space)
		.AddKey(Key::Mouse::right)
		.AddKey(Key::Gamepad::A);

	P("fire");
	fire = InputAction<float>::Create()
		.AddKey(Key::FloatKey::mouseScrollDirection)
		.AddKey(Key::FloatKey::leftTrigger);

	P("walk");
	walk = InputAction<glm::vec2>::Create()
		.AddKey(Key::VectorKey::WASD)
		.AddKey(Key::VectorKey::leftStick);

}

void GameInputs::OnGameEnd()
{
	jump.Destroy();
	fire.Destroy();
	walk.Destroy();
}