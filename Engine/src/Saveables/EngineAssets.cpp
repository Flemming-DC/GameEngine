
#include "EngineAssets.h"
#include "UuidCreator.h"

// If the shader is stored in the register, then it somehow grows darker

Shader EngineAssets::solidColorShader;
Material EngineAssets::gizmoMaterial;


void EngineAssets::Setup()
{
	solidColorShader.Setup("res/shaders/SolidColor.shader");
	gizmoMaterial.Setup(EngineAssets::SolidColorShader(), // defaults to green color
		{ {"u_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, {"u_MVP", Material::MissingUniform()} });
}












