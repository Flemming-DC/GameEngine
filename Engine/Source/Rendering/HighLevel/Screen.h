#pragma once
#include <utility>
#include "OpenGlExternal.h"
#include "GlmTools.h"
#include "ShortHands.h"
#include "Key.h"

enum class CursorMode
{
	normal, hidden, locked,
};

class Screen
{
public:
	Shorts;
	static void ApplyCursorState();

	static void SetCursorMode(CursorMode cursorMode);
	static void SetTitle(string gameTitle);

	static vec3 ToWorldPosition(vec2 normalizedScreenPosition, bool* foundDepth = nullptr);
	static vec2 FromWorldPosition(vec3 worldPosition);
	static vec3 ToWorldPosition(
		vec2 screenPos, vec2 screenMinCorner, vec2 screenMaxCorner,
		vec3 cameraPos, quat cameraRot, vec3 cameraScale, bool* foundDepth = nullptr);
	static vec2 FromWorldPosition(vec3 worldPosition, vec3 cameraPos, quat cameraRot, vec3 cameraScale);
	// normalize by screen width and height. Set origin to bottom-left.
	static vec2 NormalizeGameScreenPosition(glm::vec2 screenPosition);

private:
	static CursorMode cursorMode;

	// this should be able to find the depth, iff the cursor is in window.
	static vec3 ToWorldPosition(vec2 normalizedScreenPosition, mat4 projectionView, bool* foundDepth = nullptr);
	static vec2 FromWorldPosition(vec3 worldPosition, mat4 projectionView);
};

