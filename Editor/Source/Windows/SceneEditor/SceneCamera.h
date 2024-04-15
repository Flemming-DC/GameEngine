#pragma once
#include "GlmTools.h"
#include "imgui/imgui.h"

namespace Editor
{
	class SceneCamera
	{
	public:
		static void UpdateCamera();
		static glm::vec3 MouseWorldPosition();
		static glm::vec2 MouseWorldPosition2D() { return (glm::vec2)MouseWorldPosition(); }
		
		static void SetScreenBoundary(ImVec2 minSceneCorner, ImVec2 maxSceneCorner);
		static glm::vec2 ToWorldPosition(glm::vec2 screenPosition);
		static glm::vec2 FromWorldPosition(glm::vec2 worldPosition);

		static glm::vec3 Position();
		static glm::quat Rotation();
		static glm::vec3 Scale();
	};
}
