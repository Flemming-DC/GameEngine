#include "Time_.h"
#include <chrono>

static std::chrono::steady_clock::time_point engineStart;
static std::chrono::steady_clock::time_point gameStart;
static std::chrono::steady_clock::time_point previous;
static std::chrono::steady_clock::time_point current; // time of this frame

void Time::Setup()
{
	engineStart = std::chrono::high_resolution_clock::now();
	gameStart = std::chrono::high_resolution_clock::now();
	previous = gameStart;
	current = gameStart;
}

void Time::GameSetup()
{
	gameStart = std::chrono::high_resolution_clock::now();
	previous = gameStart;
	current = gameStart;
}

void Time::Update()
{
	previous = current;
	current = std::chrono::high_resolution_clock::now();
}

float Time::Delta()
{
	return std::chrono::duration<float>(current - previous).count();
}

float Time::Now()
{
	return std::chrono::duration<float>(current - gameStart).count();
}
/*
float Time::EngineNow()
{
	return std::chrono::duration<float>(current - engineStart).count();
}*/

double Time::ProfilerNow()
{
	return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - engineStart).count();
}
