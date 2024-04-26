#pragma once
#include "ImGuiTools.h"

namespace Editor
{
	class Gizmo
	{
	public:
		// GeoDrawing can only be used inside while drawing the scenewindow, and thus only in the scene editor.
		// evt. add Delay::ToDrawGizmo in order to circumvent this.
		static void DrawPolygon(std::vector<glm::vec2> positions, glm::vec4 color, float thickness);	
		static void DrawLines(std::vector<glm::vec2> positions, glm::vec4 color, float thickness);



	};
}

