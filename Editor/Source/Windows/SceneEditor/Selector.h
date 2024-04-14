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
		static Event<std::vector<Entity*>> onSelected; // < selection >
		static void Update();
		static std::vector<Entity*> Selection();

	private:
		static void StartSelecting();
		static void UpdateSelectionBox();
		static void FinishSelecting();
		
		static void BoxSelect();
		static void ClickSelect();

		static std::vector<Entity*> GetOverlaps(glm::vec2 selectionBoxCenter, glm::vec2 selectionBoxSize);
	};
}

