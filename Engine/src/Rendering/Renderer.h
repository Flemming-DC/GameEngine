#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Gizmo.h"

class Renderer
{
public:
	static void Draw();
	static void SetupGrid2D(float gridScale);
	static void ShutDown();

private:
	static bool showBlackScreenDebugInfo;
	static Gizmo horizontalGrid;
	static Gizmo verticalGrid;

	static bool ScreenIsBlack();
	
};

