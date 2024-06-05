#include "Delay.h"
#include "Time_.h"

Shorts;

// ------------ internal state management ------------ //
static Event<> onFrameEnd;
static vector<pair<float, std::function<void()>>> forSeconds; // delayDuration, function

void Delay::Update()
{
	onFrameEnd.Invoke();
	onFrameEnd.Clear();

	for (int i = forSeconds.size() - 1; i >= 0; i--)
	{
		forSeconds[i].first -= Time::Delta();
		if (forSeconds[i].first > 0)
			continue;
		forSeconds[i].second();
		Tools::RemoveIndex(forSeconds, i);
	}

}

void Delay::OnSceneEnd()
{
	forSeconds.clear();
}


// ------------ used by game ------------ //

void Delay::ToFrameEnd(const std::function<void()>& function)
{
	onFrameEnd.Add(function);
}

void Delay::ForSeconds(float duration, const std::function<void()>& function)
{
	forSeconds.push_back({ duration, function });
}
