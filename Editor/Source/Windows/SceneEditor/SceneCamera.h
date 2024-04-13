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
		static void SetScreenPosition(ImVec2 minSceneCorner, ImVec2 maxSceneCorner);

		static glm::vec3 Position();
		static glm::quat Rotation();
		static glm::vec3 Scale();
	};
}
