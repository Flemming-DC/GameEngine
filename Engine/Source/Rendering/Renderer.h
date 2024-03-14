#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "Gizmo.h"
#include "FrameBuffer.h"
#include "OpenGlSetup.h"
#include <stduuid/uuid.h>


class Renderer
{
public:
	struct RenderResult { unsigned int textureOpenGlid; int width; int height; };

	static void DrawToScreen();
	static RenderResult DrawToFrameBuffer(); // returns the texture_openGLid that contains the render result.
	static void SetupGrid2D(float gridScale);
	static void ShutDown();
	//static void ToggleGameWindow() { OpenGlSetup::ToggleGlfwWindow(); }
	static void ShowWindow(bool show);
	static bool IsWindowVisible();


private:
	static bool showBlackScreenDebugInfo;
	static uuids::uuid horizontalGridID;
	static uuids::uuid verticalGridID;
	static FrameBuffer frameBuffer;

	
};

