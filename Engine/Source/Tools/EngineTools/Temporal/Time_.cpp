#include "Time_.h"
#include <chrono>

// if chrono is bugging, then you can use glfwGetTime instead
static std::chrono::steady_clock::time_point editorStart;
static std::chrono::steady_clock::time_point gameStart;
static std::chrono::steady_clock::time_point previous;
static std::chrono::steady_clock::time_point current;

void Time::Setup()
{
	editorStart = std::chrono::high_resolution_clock::now();
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

float Time::EditorNow()
{
	return std::chrono::duration<float>(current - editorStart).count();
}

