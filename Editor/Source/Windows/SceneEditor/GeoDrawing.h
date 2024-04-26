#pragma once
#include "ImGuiTools.h"

namespace Editor
{
	class GeoDrawing
	{
	public:

		static void DrawCircle(glm::vec2 center, float radius, glm::vec4 color, float thickness);
		static void DrawRectangle(glm::vec2 center, glm::vec2 size, glm::vec4 color, float thickness);
		static void DrawPolygon(std::vector<glm::vec2> positions, glm::vec4 color, float thickness);

	};
}

