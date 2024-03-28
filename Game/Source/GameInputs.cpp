#include "GameInputs.h"

InputAction<bool>& GameInputs::jump = InputAction<bool>(0);
InputAction<float>& GameInputs::fire = InputAction<float>(0);
InputAction<glm::vec2>& GameInputs::walk = InputAction<glm::vec2>(0);


void GameInputs::OnGameStart()
{

	jump = InputAction<bool>::Create()
		.AddKey(Keyboard::space)
		.AddKey(Mouse::right)
		.AddKey(Gamepad::A);

	fire = InputAction<float>::Create()
		.AddKey(FloatKey::mouseScrollDirection)
		.AddKey(FloatKey::leftTrigger);

	walk = InputAction<glm::vec2>::Create()
		.AddKey(VectorKey::WASD)
		.AddKey(VectorKey::leftStick);


}
