#pragma once
#include "Shader.h"
#include "Material.h"
#include <stduuid/uuid.h>

class EngineAssets
{
public:
	static void Setup();
	static Shader& SolidColorShader() { return solidColorShader; }
	static Material& GreenGizmoMaterial() { return gizmoMaterial; }

private:
	static Shader& solidColorShader; // used by gizmo
	static Material& gizmoMaterial;
};


