#pragma once
#include "InputAction.h"


/*
*/

//void InputAction<T>::Update(); // loop over all inputActions: (1) call FindState on them (2) update state, lastState, timeOfLastChange and (3) fire events.

//InputAction<T> InputAction<T>::AddKey(Keyboard key); // repeat this for each key type. The instantiations will raise an error, if you pick the wrong type.
//InputAction<T> InputAction<T>::RemoveKey(Keyboard key); // return self for repeated function call






// use IsHeldDown / GetFloat / GetVectorInput under the hood and pick the maximal state across the keys.
template<> bool InputAction<bool>::FindState()
{ 
	//return Input::IsHeldDown();
	return false;
}


template<> float InputAction<bool>::Magnitude(bool state) { return state; }
template<> float InputAction<float>::Magnitude(float state) { return std::abs(state); }
template<> float InputAction<glm::vec2>::Magnitude(glm::vec2 state) { return glm::length(state); }















