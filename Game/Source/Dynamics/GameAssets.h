#pragma once
#include "Dynamic.h"
#include "Texture.h"

class GameAssets : public Dynamic
{
public:
	static Texture& Explosion() { return explosion; }
	static Texture& Dust() { return dust; }
private:
	static Texture& explosion;
	static Texture& dust;

	void OnGameStart();


};

