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
	static Material& GizmoMaterial() { return gizmoMaterial; }
	static Mesh& SquareMesh() { return squareMesh; }

private:
	static Shader& solidColorShader; // used by gizmo
	static Material& gizmoMaterial;
	static Mesh& squareMesh;
};


