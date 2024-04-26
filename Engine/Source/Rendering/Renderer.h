#pragma once
#include "FrameBuffer.h"


class Renderer
{
public:
	Shorts
	struct RenderResult { unsigned int textureOpenGlid; int width; int height; };

	static RenderResult DrawToFrameBuffer(vec3 cameraPos, quat cameraRot, vec3 cameraScale);
	static RenderResult DrawToFrameBuffer();
	static void DrawToScreen();
	static void ShutDown();
	//static void ToggleGameWindow() { OpenGlSetup::ToggleGlfwWindow(); }
	static void ShowWindow(bool show);
	static bool IsWindowVisible();


private:
	static bool showBlackScreenDebugInfo;
	static uuid horizontalGridID;
	static uuid verticalGridID;
	static FrameBuffer frameBuffer;

	static RenderResult DrawToFrameBuffer(mat4 projectionView);
	static void DrawToScreen(mat4 projectionView);
	
};

