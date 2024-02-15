#pragma once
#include "Register.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
//#include "Scene.h"

// this is a temp thing. In a real game, it would be split by asset type
class GameAssets
{
public:
	static void Setup();
	static Shader& GetShader() { return shader; }
	static Texture& GetTexture() { return texture; }
	static Material& GetMaterial() { return material; }

private:
	static Shader& shader;
	static Texture& texture;
	static Material& material;


};

