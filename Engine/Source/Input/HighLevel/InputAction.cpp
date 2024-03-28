#include "InputAction.h"



/*
template<> InputAction<bool>& InputAction<bool>::AddKey(Keyboard key) { keyboardKeys.push_back(key); return *this; }
template<> InputAction<bool>& InputAction<bool>::AddKey(Mouse key) { mouseKeys.push_back(key); return *this; }
template<> InputAction<bool>& InputAction<bool>::AddKey(Gamepad key) { gamepadKeys.push_back(key); return *this; }
template<> InputAction<float>& InputAction<float>::AddKey(FloatKey key) { floatKeys.push_back(key); return *this; }
template<> InputAction<glm::vec2>& InputAction<glm::vec2>::AddKey(VectorKey key) { vectorKeys.push_back(key); return *this; }

template<> void InputAction<bool>::RemoveKey(Keyboard key) { Tools::Remove(keyboardKeys, key); }
template<> void InputAction<bool>::RemoveKey(Mouse key) { Tools::Remove(mouseKeys, key); }
template<> void InputAction<bool>::RemoveKey(Gamepad key) { Tools::Remove(gamepadKeys, key); }
template<> void InputAction<float>::RemoveKey(FloatKey key) { Tools::Remove(floatKeys, key); }
template<> void InputAction<glm::vec2>::RemoveKey(VectorKey key) { Tools::Remove(vectorKeys, key); }

*/

/*
template<> bool InputAction<bool>::FindState()
{
	for (const auto& k : keyboardKeys)
		if (Input::IsHeldDown(k))
			return true;
	for (const auto& k : mouseKeys)
		if (Input::IsHeldDown(k))
			return true;
	for (const auto& k : gamepadKeys)
		if (Input::IsHeldDown(k, gamepadID))
			return true;
	return false;
}


template<> float InputAction<float>::FindState()
{
	float maxMagnitude = 0; // without sign
	float maxFloat = 0; // with sign
	for (const auto& k : floatKeys)
	{
		if (Magnitude(Input::GetFloat(k, gamepadID)) > maxMagnitude)
		{
			maxFloat = Input::GetFloat(k, gamepadID);
			maxMagnitude = Magnitude(maxFloat);
		}
	}
	return maxFloat;
}

template<> glm::vec2 InputAction<glm::vec2>::FindState()
{
	float maxMagnitude = 0;
	glm::vec2 maxVector(0);
	for (const auto& k : vectorKeys)
	{
		if (Magnitude(InputVectorizer::GetVectorInput(k, gamepadID)) > maxMagnitude)
		{
			maxVector = InputVectorizer::GetVectorInput(k, gamepadID);
			maxMagnitude = Magnitude(maxVector);
		}
	}
	return maxVector;
}


template<> float InputAction<bool>::Magnitude(bool state) const { return state; }
template<> float InputAction<float>::Magnitude(float state) const { return std::abs(state); }
template<> float InputAction<glm::vec2>::Magnitude(glm::vec2 state) const { return glm::length(state); }

*/













