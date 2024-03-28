#pragma once
#include "InputAction.h"


// ------------------------- specialized declarations -------------------------

// apparently, the specialized Magnitude() needs the be declared before implemented, even though this isn't required for other functions.
template<> float InputAction<bool>::Magnitude(bool state) const;
template<> float InputAction<float>::Magnitude(float state) const;
template<> float InputAction<glm::vec2>::Magnitude(glm::vec2 state) const;



// ------------------------- static initializations -------------------------

template<typename T> unsigned int InputAction<T>::maxID = 0;

// we declare actions for generic T, so that invalid T bugs can compile and yield a good errormessage. 
// However, its the specialized actions that are actually used
template<typename T> std::unordered_map<unsigned int, std::unique_ptr<InputAction<T>>> InputAction<T>::actions;
template<> std::unordered_map<unsigned int, std::unique_ptr<InputAction<bool>>> InputAction<bool>::actions;
template<> std::unordered_map<unsigned int, std::unique_ptr<InputAction<float>>> InputAction<float>::actions;
template<> std::unordered_map<unsigned int, std::unique_ptr<InputAction<glm::vec2>>> InputAction<glm::vec2>::actions;


// ------------------------- generic impl -------------------------


template<typename T> void InputAction<T>::LateUpdate()
{
	for (const auto& [id, actionPtr] : actions)
		actionPtr->IndividualUpdate();
}

// store current and last state and the time of change obtained from FindState<bool, float, vec2>
template<typename T> void InputAction<T>::IndividualUpdate()
{
	if (id == -1)
		RaiseError("This InputAction is not initialized. Use InputAction<T>::Create() to make an initialized inputAction.");
	bool wasActive = IsPressed();
	lastState = state;
	state = FindState();
	if (IsPressed() == wasActive)
		return;

	if (BecomesPressed())
		OnPressed.Invoke();
	else if (BecomesReleased())
		OnReleased.Invoke();
	else
		RaiseError("This else clause should be impossible to reach.");
	timeOfLastPressOrRelease = Time::Now();
}

template<typename T> std::string InputAction<T>::to_string() const
{
	return logger::make_string(
		"InputAction<", Tools::TypeName<T>(), "> with \n", 
		keyboardKeys, "\n", mouseKeys, "\n", gamepadKeys, "\n", floatKeys, "\n", vectorKeys);
}


// ------------------------- specialized impl -------------------------


template<> InputAction<bool>& InputAction<bool>::AddKey(Key::Keyboard key) 
{ 
	keyboardKeys.push_back(key); 
	return *this; 
}
template<> InputAction<bool>& InputAction<bool>::AddKey(Key::Mouse key) { mouseKeys.push_back(key); return *this; }
template<> InputAction<bool>& InputAction<bool>::AddKey(Key::Gamepad key) { gamepadKeys.push_back(key); return *this; }
template<> InputAction<float>& InputAction<float>::AddKey(Key::FloatKey key) { floatKeys.push_back(key); return *this; }
template<> InputAction<glm::vec2>& InputAction<glm::vec2>::AddKey(Key::VectorKey key) { vectorKeys.push_back(key); return *this; }

template<> void InputAction<bool>::RemoveKey(Key::Keyboard key) { Tools::Remove(keyboardKeys, key); }
template<> void InputAction<bool>::RemoveKey(Key::Mouse key) { Tools::Remove(mouseKeys, key); }
template<> void InputAction<bool>::RemoveKey(Key::Gamepad key) { Tools::Remove(gamepadKeys, key); }
template<> void InputAction<float>::RemoveKey(Key::FloatKey key) { Tools::Remove(floatKeys, key); }
template<> void InputAction<glm::vec2>::RemoveKey(Key::VectorKey key) { Tools::Remove(vectorKeys, key); }




template<> bool InputAction<bool>::FindState()
{
	for (const auto& k : keyboardKeys)
		if (Input::IsPressed(k))
			return true;
	for (const auto& k : mouseKeys)
		if (Input::IsPressed(k))
			return true;
	for (const auto& k : gamepadKeys)
		if (Input::IsPressed(k, gamepadID))
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












