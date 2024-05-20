#pragma once
#include "Register.h"
#include "Engine.h"

class GameAssets : public Dynamic
{
public:
	static Texture& RocketTex() { return rocketTex; }
	static Texture& RocketFlamingTex() { return rocketFlamingTex; }
private:
	static Texture& rocketTex;
	static Texture& rocketFlamingTex;

	void OnGameStart();// { Setup(); }


};

