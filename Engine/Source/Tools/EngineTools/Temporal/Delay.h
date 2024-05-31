#pragma once
#include "Event.h"
#include <functional>

// expand width coroutines and timed delay
class Delay
{
public:
	static void Update();
	static void OnGameStop();

	static void ToFrameEnd(const std::function<void()>& function); // Delay a function call to end of frame
	static void ForSeconds(float duration, const std::function<void()>& function);

};

