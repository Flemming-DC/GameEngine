#pragma once
#include "ImGuiTools.h"

namespace Editor
{
	class GeoDrawing
	{
	public:
		// GeoDrawing can only be used inside while drawing the scenewindow, and thus only in the scene editor.
		// evt. add Delay::ToDrawGizmo in order to circumvent this.
		static void DrawPolygon(std::vector<glm::vec2> positions, glm::vec4 color, float thickness);
		static void DrawLines(std::vector<glm::vec2> positions, glm::vec4 color, float thickness);

	private:
		// these two functions are currently unused. move them to public if you wish to use them
		static void DrawCircle(glm::vec2 center, float radius, glm::vec4 color, float thickness);
		static void DrawRectangle(glm::vec2 center, glm::vec2 size, glm::vec4 color, float thickness);

	};
}

