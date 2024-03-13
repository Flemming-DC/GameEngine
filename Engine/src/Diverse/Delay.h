#pragma once
#include "Event.h"
#include <functional>

// expand width coroutines and timed delay
class Delay
{
public:
	static void ToFrameEnd(const std::function<void()>& function); // Delay a function call to end of frame
	static void CallToFrameEnd(); // Calls the functions that where delayed to end of frame, and clears the list

private:
	static Event<> onFrameEnd;
};

