#pragma once
#include <string>

namespace Literals
{
	using std::string;

	// ------- folders ------- //
	static string Res = "res/"; // root folder for all the ressources aka. all the non-c++ files
	static string Scenes = Res + "Scenes/";
	static string Shaders = Res + "Shaders/";
	static string Textures = Res + "Textures/";
	static string Namings = Res + "Namings/";
	// string idByName = res + "IdByName";
	// evt. Materials
	// evt. Meshes
	// evt. Prefabs / Entities (aka. stored entities)

	// ------- files ------- //
	static string colorShader = Shaders + "SolidColor.shader";
	static string imageShader = Shaders + "Image.shader";
	static string defaultImage = Textures + "defaultImage.png";
	
	// ------- renderObjects ------- //
	static string colorMaterialName = "color";
	static string imageMaterialName = "image";
	static string squareMeshName = "squareMesh";

	// ------- uniforms ------- //
	static string u_color = "u_color";
	static string u_textureSampler = "u_textureSampler";

}


