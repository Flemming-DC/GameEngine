#pragma once
#include "InputAction.h"



//void InputAction<T>::Update(); // loop over all inputActions: (1) call FindState on them (2) update state, lastState, timeOfLastChange and (3) fire events.

//InputAction<T> InputAction<T>::AddKey(Keyboard key); // repeat this for each key type. The instantiations will raise an error, if you pick the wrong type.
//InputAction<T> InputAction<T>::RemoveKey(Keyboard key); // return self for repeated function call




template<> glm::vec2 InputAction<glm::vec2>::FindState()
{
	float maxMagnitude = 0; 
	glm::vec2 maxVector; 
	for (const auto& k : vectorKeys)
	{
		if (Magnitude(InputVectorizer::GetVectorInput(k)) > maxMagnitude)
		{
			maxVector = InputVectorizer::GetVectorInput(k);
			maxMagnitude = Magnitude(maxVector);
		}
	}
	return maxVector;
}

template<> float InputAction<float>::FindState()
{
	float maxMagnitude = 0; // without sign
	float maxFloat = 0; // with sign
	for (const auto& k : floatKeys)
	{
		if (Magnitude(Input::GetFloat(k)) > maxMagnitude)
		{
			maxFloat = Input::GetFloat(k);
			maxMagnitude = Magnitude(maxFloat);
		}
	}
	return maxFloat;
}

template<> bool InputAction<bool>::FindState()
{
	for (const auto& k : keyboardKeys)
		if (Input::IsHeldDown(k))
			return true;
	for (const auto& k : mouseKeys)
		if (Input::IsHeldDown(k))
			return true;
	for (const auto& k : gamepadKeys)
		if (Input::IsHeldDown(k))
			return true;
	return false;
}


template<> inline float InputAction<bool>::Magnitude(bool state) { return state; }
template<> inline float InputAction<float>::Magnitude(float state) { return std::abs(state); }
template<> inline float InputAction<glm::vec2>::Magnitude(glm::vec2 state) { return glm::length(state); }















