#pragma once
#include "Dynamic.h"
#include "Texture.h"

class GameAssets : public Dynamic
{
public:
	static Texture& RocketTex() { return rocket; }
	static Texture& RocketFlamingTex() { return rocketFlaming; }
	static Texture& ExplosionTex() { return explosion; }
private:
	static Texture& rocket;
	static Texture& rocketFlaming;
	static Texture& explosion;
	
	void OnGameStart();


};

