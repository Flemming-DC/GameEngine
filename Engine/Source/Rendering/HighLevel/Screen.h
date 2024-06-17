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

	static vec3 ToWorldPosition(vec2 normalizedScreenPosition, bool* foundDepth = nullptr); // using current cam and glfw window
	static vec2 FromWorldPosition(vec3 worldPosition); // using current cam and glfw window
	static vec3 ToWorldPosition(
		vec2 screenPos, vec2 screenMinCorner, vec2 screenMaxCorner,
		vec3 cameraPos, quat cameraRot, vec3 cameraScale, bool* foundDepth = nullptr); // can be used by editor
	static vec2 FromWorldPosition(
		vec3 worldPosition, vec2 screenMinCorner, vec2 screenMaxCorner,
		vec3 cameraPos, quat cameraRot, vec3 cameraScale); // can be used by editor
	// normalize by screen width and height. Set origin to bottom-left.
	static vec2 NormalizeGameScreenPosition(glm::vec2 screenPosition);

private:
	static CursorMode cursorMode;

};

