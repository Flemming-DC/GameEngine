#pragma once
#include "Dynamic.h"
#include "Texture.h"

class GameAssets : public Dynamic
{
public:
	static Texture& RocketTex() { return rocketTex; }
	static Texture& RocketFlamingTex() { return rocketFlamingTex; }
private:
	static Texture& rocketTex;
	static Texture& rocketFlamingTex;

	void OnGameStart();


};

