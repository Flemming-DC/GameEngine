#pragma once

// evt. template bool, float, vec2
class InputAction
{
	// list of inputKeys i.e. particular elements of Keyboard, Mouse, Gamepad, InputVector according to the cuttent setup.
	// evt. replace this with InputBool, InputFloat, InputVector

	// dont do IsPressed = disjunction of IsPressed for all inputKeys, since you can get two IsPressed in a row without any IsRealesed this way

	// IsHeldDown = disjunction of IsHeldDown for all inputKeys
	// store last value IsHeldDown to recalculate IsPressed, IsReleased and to fire events.
	// Input must provide a IsHeldDown bool for all input types, include float and vector (simply compare them to zero).
	// expose a value function, a delta function and store last value to provide these
	// OnPressed, OnReleased events.

};

