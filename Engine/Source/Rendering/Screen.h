#pragma once
#include <utility>
#include "GlmTools.h"
#include "Key.h"

enum class CursorMode
{
	normal, hidden, locked,
};

class Screen
{
public:
	static void LateUpdate();

	// this should be able to find the depth, iff the cursor is in window.
	static glm::vec3 ToWorldPosition(glm::vec2 normalizedScreenPosition, bool* foundDepth = nullptr); 
	static glm::vec2 FromWorldPosition(glm::vec3 worldPosition);
	// normalize by screen width and height. Set origin to bottom-left.
	static glm::vec2 NormalizeScreenPosition(glm::vec2 screenPosition);
	static void SetCursorMode(CursorMode cursorMode);
	static void SetTitle(std::string gameTitle);

private:
	static CursorMode cursorMode;
};

