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
	Shorts
	struct RenderResult { unsigned int textureOpenGlid; int width; int height; };

	static void DrawToScreen();
	static RenderResult DrawToFrameBuffer();
	// evt. drop useGizmos and make InEditor globally available
	static void DrawToScreen(mat4 projectionView, bool useGizmos = false);
	static RenderResult DrawToFrameBuffer(vec3 cameraPos, quat cameraRot, vec3 cameraScale = vec3(1.0f), bool useGizmos = false);
	static void SetupGrid2D(float gridScale);
	static void ShutDown();
	//static void ToggleGameWindow() { OpenGlSetup::ToggleGlfwWindow(); }
	static void ShowWindow(bool show);
	static bool IsWindowVisible();


private:
	static bool showBlackScreenDebugInfo;
	static uuid horizontalGridID;
	static uuid verticalGridID;
	static FrameBuffer frameBuffer;

	
};

