#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Gizmo.h"
#include "FrameBuffer.h"
#include "Initializer.h"
#include <stduuid/uuid.h>

class Renderer
{
public:
	struct RenderResult { unsigned int textureOpenGlid; int width; int height; };

	static void DrawToScreen();
	static RenderResult DrawToFrameBuffer(); // returns the texture_openGLid that contains the render result.
	static void SetupGrid2D(float gridScale);
	static void ShutDown();

private:
	static bool showBlackScreenDebugInfo;
	static uuids::uuid horizontalGridID;
	static uuids::uuid verticalGridID;
	static FrameBuffer frameBuffer;

	static bool ScreenIsBlack();
	
};

