#include "Time_.h"
#include <chrono>

static std::chrono::steady_clock::time_point start;
static std::chrono::steady_clock::time_point previous;
static std::chrono::steady_clock::time_point current;

void Time::MakeEntities()
{
	start = std::chrono::high_resolution_clock::now();
	previous = start;
	current = start;
}

void Time::Update()
{
	previous = current;
	current = std::chrono::high_resolution_clock::now();
}

float Time::GetDelta()
{
	return std::chrono::duration<float>(current - previous).count();
}

float Time::GetCurrent()
{
	return std::chrono::duration<float>(current - start).count();
}


