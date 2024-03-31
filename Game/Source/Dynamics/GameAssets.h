#pragma once
#include "Register.h"
#include "Engine.h"

// this is a temp thing. In a real game, it would be split by asset type
class GameAssets : public Dynamic
{
public:
	static void Setup(); // static OnGameStart
	static Shader& GetShader() { return shader; }
	static Texture& GetTexture() { return texture; }
	static Material& GetMaterial() { return material; }

private:
	static Shader& shader;
	static Texture& texture;
	static Material& material;

	void OnGameStart() { Setup(); }

};

