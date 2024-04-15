#include "SelectionMover.h"
#include "Selector.h"
#include "Transform.h"

//Shorts;
using namespace Editor;




void SelectionMover::SetPosition(vec2 newPos)
{
	for (const auto& entityPtr : Selector::Selection())
	{
		entityPtr->Get<Transform>();
	}
}

void SelectionMover::SetRotation(float angle)
{

}

void SelectionMover::SetScale(vec2 newScale)
{

}
