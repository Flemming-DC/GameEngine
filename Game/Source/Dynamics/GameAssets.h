#pragma once
#include "Register.h"
#include "Engine.h"

// this is a temp thing. In a real game, it would be split by asset type
class GameAssets : public Dynamic
{
public:

private:
	//static Shader& shader;
	//static Texture& texture;
	//static Material& material;

	void OnGameStart();// { Setup(); }

	//static Shader& GetShader() { return shader; }
	//static Texture& GetTexture() { return texture; }
	//static Material& GetMaterial() { return material; }

};

