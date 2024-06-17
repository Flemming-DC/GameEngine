#pragma once
#include "FrameBuffer.h"
#include "Renderable.h"


class Renderer
{
public:
	Shorts
	struct RenderResult { unsigned int textureOpenGlid; int width; int height; };

	static RenderResult DrawToFrameBuffer(vec3 cameraPos, quat cameraRot, vec3 cameraScale, vec2 screenMinCorner, vec2 screenMaxCorner);
	static RenderResult DrawToFrameBuffer(); // not used by engine
	static void DrawToScreen();
	static void ShutDown();
	static void ShowWindow(bool show);
	static bool IsWindowVisible();

private:
	static bool showBlackScreenDebugInfo;
	static uuid horizontalGridID;
	static uuid verticalGridID;
	static FrameBuffer frameBuffer;

	static RenderResult DrawToFrameBuffer(const mat4& projectionView, const RenderBoundingBox& viewBounds);
	static void DrawToScreen(const mat4& projectionView, const RenderBoundingBox& viewBounds);
	
};

