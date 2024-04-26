#pragma once
#include "Entity.h"
#include "GlmTools.h"

namespace Editor
{
	class SelectionVisuals
	{
	public:
		static void Update();

	private:
		static void DrawSelectionBox(glm::vec2 selectionStartPosition, glm::vec2 selectionEndPosition);
		static void DrawSelection();
		static void Display(const Entity& entity);
		static void DrawPolygon(std::vector<glm::vec2> positions);

	};
}
