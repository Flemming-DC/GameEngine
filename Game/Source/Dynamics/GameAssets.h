#pragma once
#include "Dynamic.h"
#include "Texture.h"

class GameAssets : public Dynamic
{
public:
	static Texture& RocketTex() { return rocket; }
	static Texture& RocketFlamingTex() { return rocketFlaming; }
private:
	static Texture& rocket;
	static Texture& rocketFlaming;

	void OnGameStart();


};

