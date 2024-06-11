#pragma once


class Time
{
public:
	static void Setup();
	static void GameSetup();
	static void Update();
	static float Delta();
	static float Now(); // time since game start
	//static float EngineNow(); // time since engine start
	static double ProfilerNow(); // precise time since engine start
	
};


