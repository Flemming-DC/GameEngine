
#include "EngineAssets.h"
#include "UuidCreator.h"

// If the shader is stored in the register, then it somehow grows darker

Shader& EngineAssets::solidColorShader = Shader();
Material& EngineAssets::gizmoMaterial = Material();


void EngineAssets::Setup()
{
	Log("EngineAssets::Setup");
	// parameters
	std::map<std::string, std::any> uniformsByName = {
		{"u_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
		{"u_MVP", Material::MissingUniform()} };

    // assets
	solidColorShader = Shader::register_.Add("res/shaders/SolidColor.shader");
	gizmoMaterial = Material::register_.Add(EngineAssets::SolidColorShader(), uniformsByName);

/*
	solidColorShader.Setup("res/shaders/SolidColor.shader");
	gizmoMaterial.Setup(EngineAssets::SolidColorShader(), // defaults to green color
		{ {"u_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, {"u_MVP", Material::MissingUniform()} });
*/
}












