#pragma once
#include "Event.h"
#include "GlmTools.h"
#include "Entity.h"

namespace Editor
{
	class Selector
	{
		Shorts;
	public:
		static Event<vector<uuid>> onSelected; // vector<uuid> = vector<entityId> = selection 
		static void Start();
		static void Update();
		static vector<uuid> Selection();
		static vec2 SelectionStartPosition(); // or dragStartPosition
		static bool IsDraggingSelection();
		static bool IsSelecting();

		static void SelectFromHierachy(uuid id); // only used by Hierachy, not by Selector itself
		static bool IsSelected(uuid id);

	private:
		static void StartSelecting();
		static void UpdateSelectionBox();
		static void FinishSelecting();
		
		static void BoxSelect();
		static void ClickSelect();

		static vector<Entity*> GetOverlaps(vec2 selectionBoxCenter, vec2 selectionBoxSize);
		static bool ClickedOnSelection();
	};
}

