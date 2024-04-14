#pragma once
//#include "Gizmo.h"
#include "GlmTools.h"
#include "imgui/imgui.h"

namespace Editor
{
	class SelectionVisuals
	{
	public:
		void DrawSelectionBox(glm::vec2 selectionStartPosition, glm::vec2 selectionEndPosition);
		void DrawSelection();
	private:
		//Gizmo selectionBox;
		//Gizmo
		// draw 
		//void DrawRectUnfilled(ImVec2 topLeft, ImVec2 bottomRight, ImU32 color);
	};
}
