
#include "EngineAssets.h"
#include "UuidCreator.h"

// If the shader is stored in the register, then it somehow grows darker

Shader EngineAssets::solidColorShader;


void EngineAssets::Setup()
{
	solidColorShader.Setup("res/shaders/SolidColor.shader");
}












