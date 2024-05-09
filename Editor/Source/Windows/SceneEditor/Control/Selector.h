#pragma once
#include "Event.h"
#include "GlmTools.h"
#include "Entity.h"
//#include "Collider.h"

namespace Editor
{
	class Selector
	{
	public:
		static Event<std::vector<uuids::uuid>> onSelected; // < selection >
		static void Start();
		static void Update();
		static std::vector<uuids::uuid> Selection();
		static glm::vec2 SelectionStartPosition(); // or dragStartPosition
		static bool IsDraggingSelection();
		static bool IsSelecting();

	private:
		static void StartSelecting();
		static void UpdateSelectionBox();
		static void FinishSelecting();
		
		static void BoxSelect();
		static void ClickSelect();

		static std::vector<Entity*> GetOverlaps(glm::vec2 selectionBoxCenter, glm::vec2 selectionBoxSize);
		static bool ClickedOnSelection();
	};
}

