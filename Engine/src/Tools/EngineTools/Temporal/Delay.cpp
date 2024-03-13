#include "Delay.h"

Event<> Delay::onFrameEnd;


void Delay::ToFrameEnd(const std::function<void()>& function)
{
	onFrameEnd.Add(function);
}

void Delay::CallToFrameEnd()
{
	onFrameEnd.Invoke();
	onFrameEnd.Clear();
}
