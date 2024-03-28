#include "GameInputs.h"

InputAction<bool>& GameInputs::jump = InputAction<bool>::Create();
InputAction<float>& GameInputs::fire = InputAction<float>::Create();
InputAction<glm::vec2>& GameInputs::walk = InputAction<glm::vec2>::Create();

void GameInputs::OnGameStart()
{
	jump.AddKey(Key::Keyboard::space)
		.AddKey(Key::Mouse::right)
		.AddKey(Key::Gamepad::A);

	fire.AddKey(Key::FloatKey::mouseScrollDirection)
		.AddKey(Key::FloatKey::leftTrigger);

	walk.AddKey(Key::VectorKey::WASD)
		.AddKey(Key::VectorKey::leftStick);
	//walk.gamepadID = 1; // second gamepad
}

void GameInputs::OnGameEnd()
{
	jump.Destroy();
	fire.Destroy();
	walk.Destroy();
}