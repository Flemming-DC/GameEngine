#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Gizmo.h"
#include <stduuid/uuid.h>

class Renderer
{
public:
	static void Draw();
	static void SetupGrid2D(float gridScale);
	static void ShutDown();

private:
	static bool showBlackScreenDebugInfo;
	static uuids::uuid horizontalGridID;
	static uuids::uuid verticalGridID;

	static bool ScreenIsBlack();
	
};

