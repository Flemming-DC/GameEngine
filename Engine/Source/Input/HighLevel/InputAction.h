#pragma once

// get to know template classes with header, cpp, impl.h files and with explicit bool, float, vec2 instantiations

// evt. template bool, float, vec2
class InputAction
{
	// gamepadID as member variable
	// enabled (wait with this until you start working on the inputMode)
	// list of inputKeys i.e. particular elements of Keyboard, Mouse, Gamepad, InputVector according to the cuttent setup.
	// evt. replace this with InputBool, InputFloat, InputVector


	// AddKey: overloads for different kinds of keys. Allow different overloads for bool, float, vec2
	// RemoveKey (overloads for different kinds of keys)

	// GetStateInternal<bool, float, vec2> returns the current state. 
	//		It uses IsHeldDown, GetFloat, GetVectorInput under the hood and it picks the maximal state across the keys.
	// store current and last state and time of change obtained from GetStateInternal<bool, float, vec2>
	// contruct State(), Delta(), IsActive(), IsPressed(), IsReleased(), TimeOfLastPress(), TimeOfLastRelease() based of current and last state
	// fire OnPressed and OnReleased events based of current and last state


};

// template specializations and instantiations in InputAction_impl.h

// ---------------

// dont do IsPressed = disjunction of IsPressed for all inputKeys, since you can get two IsPressed in a row without any IsRealesed this way

// IsHeldDown = disjunction of IsHeldDown for all inputKeys
// store last value IsHeldDown to recalculate IsPressed, IsReleased and to fire events.
// Input must provide a IsHeldDown bool for all input types, include float and vector (simply compare them to zero).
// expose a value function, a delta function and store last value to provide these
// OnPressed, OnReleased events.
