#include "GameInputs.h"
//using namespace GameInputs;




namespace GameInputs
{
	static auto& jump = InputAction<bool>::Create()
		.AddKey(Key::Keyboard::space)
		.AddKey(Key::Mouse::right)
		.AddKey(Key::Gamepad::A);

	static auto& fire = InputAction<float>::Create()
		.AddKey(Key::FloatKey::mouseScrollVelocity)
		.AddKey(Key::FloatKey::leftTrigger);

	static auto& move = InputAction<glm::vec2>::Create()
		.AddKey(Key::VectorKey::WASD)
		.AddKey(Key::VectorKey::leftStick);
	//walk.gamepadID = 1; // second gamepad

	InputAction<bool>& Jump() { return jump; };
	InputAction<float>& Fire() { return fire; };
	InputAction<glm::vec2>& Move() { return move; };

}

