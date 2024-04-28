#pragma once
#include <string>

namespace EngineFiles
{
	using std::string;

	// ------- folders ------- //
	static string res = "res/"; // root folder for all the ressources aka. all the non-c++ files
	static string scenes = res + "Scenes/";
	static string shaders = res + "Shaders/";
	static string textures = res + "Textures/";
	// string idByName = res + "IdByName";
	// evt. Materials
	// evt. Meshes
	// evt. Prefabs / Entities (aka. stored entities)

	// ------- files ------- //
	static string imageShader = shaders + "Image.shader";
	static string colorShader = shaders + "SolidColor.shader";
	
}


