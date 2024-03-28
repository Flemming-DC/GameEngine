#pragma once
#include <memory>
#include "GlmTools.h"
#include "stringTools.h"
#include "Event.h"
#include "InputVectorizer.h"
#include "Input.h"
#include "InputEnums.h" // move some headers into impl file
#include "Time_.h"

// get to know template classes with header, cpp, impl.h files and with explicit bool, float, vec2 instantiations

// T is one of bool, float, vec2
template <typename T>
//typename std::enable_if<T is bool, float, vec2>::type
class InputAction
{
public:
	Event<> OnPressed;
	Event<> OnReleased;
	int gamepadID = -1; // singleplayerID appears again 
	// enabled (wait with this until you start working on the inputMode)
	
	static InputAction<T>& Create() 
	{ 
		maxID++;
		actions.emplace(maxID, std::make_unique<InputAction<T>>(maxID));
		return *(actions[maxID]);
	}
	//static void Destroy(const InputAction<T>& action) { Tools::RemoveKey(actions, action.id); } 
	static void LateUpdate(); // loop over all inputActions: (1) call FindState on them (2) update state, lastState, timeOfLastChange and (3) fire events.

	// returns itself to enable elegant repeated calling
	// defaults to raising an error. the valid input are overridden in the specializations
	InputAction<T>& AddKey(Keyboard key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(Mouse key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(Gamepad key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(FloatKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(VectorKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }

	void RemoveKey(Keyboard key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(Mouse key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(Gamepad key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(FloatKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(VectorKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }


	T State() const { return state; } // these functions are based of current and last state
	T Delta() const { return state - lastState; }
	bool IsActive() const { return Magnitude(state) > noiseThreshold; }
	bool IsPressed() const { return Magnitude(state) > noiseThreshold && Magnitude(lastState) <= noiseThreshold; };
	bool IsReleased() const { return Magnitude(state) <= noiseThreshold && Magnitude(lastState) > noiseThreshold; };
	float ActivationDuration() const { return Time::Now() - timeOfLastActivationChange; } // time since IsActive changed


	//~InputAction() { P("destroyed"); } // this destructor is only for testing
	InputAction(unsigned int id_) : id(id_), state(0), lastState(0) {} // only the factory method can instantiate this class // this should be private
private:
	Shorts;
	
	T state;
	T lastState; // state at last frame
	float timeOfLastActivationChange = 0;
	inline const static float noiseThreshold = 0.0001f; // this should be smaller than noiseThreshold from glfwInput
	vector<Keyboard> keyboardKeys;
	vector<Mouse> mouseKeys;
	vector<Gamepad> gamepadKeys; // evt. combine these three into boolKeys
	vector<FloatKey> floatKeys;
	vector<VectorKey> vectorKeys; // rename to vectorKeys
	vector<uint> keys; // alternative to seperate list for each key type
	
	static map_uo<uint, unique_ptr<InputAction>> actions;
	uint id; 
	static uint maxID;

	void IndividualUpdate(); // update an individual action
	T FindState() {} // evt. rename to Update.
	// use IsHeldDown / GetFloat / GetVectorInput under the hood and pick the maximal state across the keys.
	// store current and last state and time of change obtained from GetStateInternal<bool, float, vec2>
	
	float Magnitude(T state) const { return 0; } // could e.g. be public. This function is implemented seperately for each type

};

//template<typename T> std::unordered_map<unsigned int, std::unique_ptr<InputAction<T>>> InputAction<T>::actions;
template<typename T> unsigned int InputAction<T>::maxID = 0;
//template<typename T> float InputAction<T>::noiseThreshold = 0.0001f;

template<> std::unordered_map<unsigned int, std::unique_ptr<InputAction<bool>>> InputAction<bool>::actions;
template<> std::unordered_map<unsigned int, std::unique_ptr<InputAction<float>>> InputAction<float>::actions;
template<> std::unordered_map<unsigned int, std::unique_ptr<InputAction<glm::vec2>>> InputAction<glm::vec2>::actions;

//template<typename T> unsigned int InputAction<bool>::maxID = 0;



/*
template<> InputAction<bool>& InputAction<bool>::AddKey(Keyboard key);
template<> InputAction<bool>& InputAction<bool>::AddKey(Mouse key);
template<> InputAction<bool>& InputAction<bool>::AddKey(Gamepad key);
template<> InputAction<float>& InputAction<float>::AddKey(FloatKey key);
template<> InputAction<glm::vec2>& InputAction<glm::vec2>::AddKey(VectorKey key);

template<> void InputAction<bool>::RemoveKey(Keyboard key);
template<> void InputAction<bool>::RemoveKey(Mouse key);
template<> void InputAction<bool>::RemoveKey(Gamepad key);
template<> void InputAction<float>::RemoveKey(FloatKey key);
template<> void InputAction<glm::vec2>::RemoveKey(VectorKey key);


template<> bool InputAction<bool>::FindState();
template<> float InputAction<float>::FindState();
template<> glm::vec2 InputAction<glm::vec2>::FindState();

template<> float InputAction<bool>::Magnitude(bool state) const;
template<> float InputAction<float>::Magnitude(float state) const;
template<> float InputAction<glm::vec2>::Magnitude(glm::vec2 state) const;
*/
template<> float InputAction<bool>::Magnitude(bool state) const;
template<> float InputAction<float>::Magnitude(float state) const;
template<> float InputAction<glm::vec2>::Magnitude(glm::vec2 state) const;

#include "InputAction_impl.h"




// template specializations and instantiations in InputAction_impl.h

// ---------------

// dont do IsPressed = disjunction of IsPressed for all inputKeys, since you can get two IsPressed in a row without any IsRealesed this way

// IsHeldDown = disjunction of IsHeldDown for all inputKeys
// store last value IsHeldDown to recalculate IsPressed, IsReleased and to fire events.
// Input must provide a IsHeldDown bool for all input types, include float and vector (simply compare them to zero).
// expose a value function, a delta function and store last value to provide these
// OnPressed, OnReleased events.
