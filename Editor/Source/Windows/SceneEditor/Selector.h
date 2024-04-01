#pragma once

class Entity;
namespace Editor
{
	class Selector
	{
	public:
		void Update();

	private:
		void StartSelecting();
		void UpdateSelectionBox();
		void FinishSelecting();
		void SelectUnitsInBox();
		Entity* TryGetSelectableByClick(); // ????
		void SelectUnitType(); // ????
		void SelectSingleUnit(const Entity& entity);
		void ClearSelectionUnlessShiftSelecting();
		void AddToSelection(const Entity& entity);
	};
}

