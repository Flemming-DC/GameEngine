#include "GameInputs.h"

InputAction<bool>* GameInputs::jump = nullptr;
InputAction<float>* GameInputs::fire = nullptr;
InputAction<glm::vec2>* GameInputs::walk = nullptr;

void GameInputs::OnGameStart()
{
	jump = &InputAction<bool>::Create()
		.AddKey(Key::Keyboard::space)
		.AddKey(Key::Mouse::right)
		.AddKey(Key::Gamepad::A);

	fire = &InputAction<float>::Create()
		.AddKey(Key::FloatKey::mouseScrollVelocity)
		.AddKey(Key::FloatKey::leftTrigger);

	walk = &InputAction<glm::vec2>::Create()
		.AddKey(Key::VectorKey::WASD)
		.AddKey(Key::VectorKey::leftStick);
	//walk.gamepadID = 1; // second gamepad
}

void GameInputs::OnGameEnd()
{
	jump->Destroy();
	fire->Destroy();
	walk->Destroy();
}

