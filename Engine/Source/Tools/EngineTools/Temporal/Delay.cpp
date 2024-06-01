#include "Delay.h"
#include "Time_.h"

Shorts;

static Event<> onFrameEnd;
static vector<pair<float, std::function<void()>>> forSeconds; // delayDuration, function
//static vector<pair<float, std::function<void()>>> forSecondsReadyToCall;

void Delay::Update()
{
	onFrameEnd.Invoke();
	onFrameEnd.Clear();

	//forSecondsReadyToCall.clear();
	for (int i = forSeconds.size() - 1; i >= 0; i--)
	{
		forSeconds[i].first -= Time::Delta();
		if (forSeconds[i].first > 0)
			continue;
		forSeconds[i].second();
		Tools::RemoveIndex(forSeconds, i);
	}

	/*
	for (auto& [duration, function] : forSeconds)
	{
		duration -= Time::Delta();
		if (duration > 0)
			continue;
		forSecondsReadyToCall.push_back(function);
		//function();
	}
	//Tools::Remove(forSeconds, [](auto& pair) { return pair.first <= 0; });
	for (auto& pair : forSecondsReadyToCall)
	{
		pair.second();
		Tools::Remove(forSeconds, pair);
	}
	*/
}

void Delay::OnSceneEnd()
{
	forSeconds.clear();
}


void Delay::ToFrameEnd(const std::function<void()>& function)
{
	onFrameEnd.Add(function);
}

void Delay::ForSeconds(float duration, const std::function<void()>& function)
{
	forSeconds.push_back({ duration, function });
}
