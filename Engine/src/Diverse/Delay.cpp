#include "Delay.h"

Event<> Delay::onFrameEnd;


void Delay::ToFrameEnd(const std::function<void()>& function)
{
	onFrameEnd.Add(function);
}

void Delay::OnFrameEnd()
{
	onFrameEnd.Invoke();
	onFrameEnd.Clear();
}
