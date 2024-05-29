#include "Delay.h"
#include "Time_.h"

Shorts;

static Event<> onFrameEnd;
static vector<pair<float, std::function<void()>>> forSeconds; // delayDuration, function


void Delay::Update()
{
	onFrameEnd.Invoke();
	onFrameEnd.Clear();

	for (auto& [duration, function] : forSeconds)
	{
		duration -= Time::Delta();
		if (duration > 0)
			continue;
		function();
	}
	Tools::Remove(forSeconds, [](auto& pair) { return pair.first <= 0; });

}

void Delay::ToFrameEnd(const std::function<void()>& function)
{
	onFrameEnd.Add(function);
}

void Delay::ForSeconds(float duration, const std::function<void()>& function)
{
	forSeconds.push_back({ duration, function });
}
