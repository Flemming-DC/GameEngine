#pragma once
#include "Entity.h"
#include "GlmTools.h"
#include "imgui/imgui.h"

namespace Editor
{
	class SelectionVisuals
	{
	public:
		static void DrawSelectionBox(glm::vec2 selectionStartPosition, glm::vec2 selectionEndPosition);
		static void DrawSelection();

	private:
		static void Display(const Entity& entity);
		static void DrawPolygon(std::vector<glm::vec2> positions, ImU32 color);

	};
}
