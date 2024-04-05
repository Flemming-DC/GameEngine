#pragma once
#include "Event.h"
#include "Collider.h"

namespace Editor
{
	class Selector
	{
	public:
		static Event<std::vector<Collider*>> onSelected;
		static void Update();

	private:
		static void StartSelecting();
		static void UpdateSelectionBox();
		static void FinishSelecting();
		
		static void BoxSelect();
		static void ClickSelect();
	};
}

