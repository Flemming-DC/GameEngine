#pragma once
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include <stduuid/uuid.h>

class EngineAssets
{
public:
	static void Setup();
	static Shader& SolidColorShader() { return solidColorShader; }
	static Material& DefaultMaterial() { return defaultMaterial; }
	static Material& ImageMaterial() { return imageMaterial; }
	static Mesh& SquareMesh() { return squareMesh; }

private:
	static Shader& solidColorShader; // used by gizmo
	static Material& defaultMaterial;
	static Mesh& squareMesh;
	static Shader& imageShader;
	static Texture& defualtImage;
	static Material& imageMaterial;
};


