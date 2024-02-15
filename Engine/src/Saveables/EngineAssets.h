#pragma once
#include "Shader.h"
#include "Material.h"
#include <stduuid/uuid.h>

class EngineAssets
{
public:
	static void Setup();
	static const Shader& SolidColorShader() { return solidColorShader; }
	static const Material& GreenGizmoMaterial() { return gizmoMaterial; }

private:
	static Shader solidColorShader; // used by gizmo
	static Material gizmoMaterial;
};


