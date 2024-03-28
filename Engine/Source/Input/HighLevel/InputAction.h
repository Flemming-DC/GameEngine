#pragma once
#include <memory>
#include "GlmTools.h"
#include "stringTools.h"
#include "Event.h"
#include "InputVectorizer.h"
#include "Input.h"
#include "Key.h" // move some headers into impl file
#include "Time_.h"


template <typename T>
class InputAction
{
public:
	Event<> OnPressed;
	Event<> OnReleased;
	int gamepadID = -1; // singleplayerID appears again 
	// enabled (wait with this until you start working on the inputMode)
	
	static InputAction<T>& Create() 
	{ 
		P(1);
		if (typeid(T) != typeid(bool) && typeid(T) != typeid(float) && typeid(T) != typeid(glm::vec2))
			RaiseError("Unrecognized type. Expected bool, float or glm::vec2. Received ", Tools::TypeName<T>());
		maxID++;
		actions.emplace(maxID, std::make_unique<InputAction<T>>(maxID));
		return *(actions[maxID]);
	}
	InputAction(unsigned int id_) : id(id_), state(0), lastState(0) {} // used to provide an uninitialiized instance
	void Destroy() { Tools::RemoveKey(actions, this->id); }
	static void LateUpdate(); // loop over all inputActions: (1) call FindState on them (2) update state, lastState, timeOfLastChange and (3) fire events.

	// returns itself to enable elegant repeated calling. defaults to raising an error. the valid input are overridden in the specializations
	InputAction<T>& AddKey(Key::Keyboard key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(Key::Mouse key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(Key::Gamepad key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(Key::FloatKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	InputAction<T>& AddKey(Key::VectorKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }

	void RemoveKey(Key::Keyboard key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(Key::Mouse key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(Key::Gamepad key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(Key::FloatKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }
	void RemoveKey(Key::VectorKey key) { RaiseError("Key ", key, " doesn't yield input of type ", Tools::TypeName<T>(), " as required by this inputAction"); }


	T State() const { return state; } // these functions are based of current and last state
	T Delta() const { return state - lastState; }
	bool IsActive() const { return Magnitude(state) > noiseThreshold; }
	bool IsPressed() const { return Magnitude(state) > noiseThreshold && Magnitude(lastState) <= noiseThreshold; };
	bool IsReleased() const { return Magnitude(state) <= noiseThreshold && Magnitude(lastState) > noiseThreshold; };
	float ActivationDuration() const { return Time::Now() - timeOfLastActivationChange; } // time since IsActive changed


private:
	Shorts;

	T state;
	T lastState; // state at last frame
	float timeOfLastActivationChange = 0;
	vector<Key::Keyboard> keyboardKeys;
	vector<Key::Mouse> mouseKeys;
	vector<Key::Gamepad> gamepadKeys; // evt. combine these three into boolKeys
	vector<Key::FloatKey> floatKeys;
	vector<Key::VectorKey> vectorKeys; // rename to vectorKeys
	vector<uint> keys; // alternative to seperate list for each key type
	static map_uo<uint, unique_ptr<InputAction>> actions;
	inline const static float noiseThreshold = 0.0001f; // this should be smaller than noiseThreshold from glfwInput
	static uint maxID;
	uint id;

	void IndividualUpdate(); // update an individual action
	T FindState() {} // gets the state of the maximally activated key. 
	float Magnitude(T state) const { return 0; } // could e.g. be public. This function is implemented seperately for each type

};


#include "InputAction_impl.h"



