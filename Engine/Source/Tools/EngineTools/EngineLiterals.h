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
	static string Entities = Res + "Entities/";
	static string Sounds = Res + "Sounds/";
	static string Log = Res + "Log/";
	

	// ------- files ------- //
	static string colorShader = Shaders + "Builtin/SolidColor.shader";
	static string imageShader = Shaders + "Builtin/Image.shader";
	static string defaultImage = Textures + "Builtin/defaultImage.png";
	
	// ------- renderObjects ------- //
	static string colorMaterialName = "color";
	static string imageMaterialName = "image";
	static string squareMeshName = "squareMesh";

	// ------- uniforms ------- //
	static string u_color = "u_color";
	static string u_textureSampler = "u_textureSampler";

}


