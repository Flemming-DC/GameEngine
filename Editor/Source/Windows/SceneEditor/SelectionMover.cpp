#include "SelectionMover.h"
#include "Selector.h"
#include "Transform.h"
#include "EditorInputs.h"
#include "SceneCamera.h"
#include "Time_.h"

Shorts;
using namespace Editor;

enum class Control { position, rotation, scale };
static Control control = Control::position;
static vec2 lastMouseWorldPosition2D = vec2();
static float moveSpeed = 1.5f; // evt. multiply by zoom (get the zoom from camera, not input)

void SelectionMover::Update()
{
	UpdateControl();
	vec2 delta = Delta();
	Move(delta);
}

void SelectionMover::UpdateControl()
{
	if (EditorInputs::ControlPosition().BecomesPressed())
		control = Control::position;
	else if (EditorInputs::ControlRotation().BecomesPressed())
		control = Control::rotation;
	else if (EditorInputs::ControlScale().BecomesPressed())
		control = Control::scale;
}

vec2 SelectionMover::Delta()
{
	bool dragging = Selector::IsDraggingSelection();
	vec2 delta;

	vec2 mouseWorldPosition2D = SceneCamera::MouseWorldPosition2D();
	if (dragging)
		delta = (mouseWorldPosition2D - lastMouseWorldPosition2D);// *moveSpeed* Time::Delta();
	else
		delta = EditorInputs::SelectionMoveDirection().State() * moveSpeed * Time::Delta();
	lastMouseWorldPosition2D = mouseWorldPosition2D;
	return delta;
}

void SelectionMover::Move(vec2 delta)
{
	for (const auto& entityPtr : Selector::Selection())
	{
		if (entityPtr == nullptr)
			RaiseError("entityPtr is nullptr");
		Transform& transform = entityPtr->Get<Transform>();

		// skip descendants, since they will already be moving due to moving their parents
		bool isDescendant = false;
		for (const auto& other : Selector::Selection())
		{
			isDescendant = transform.IsDescendantOf(other->Get<Transform>());
			if (isDescendant)
				break;
		}
		if (isDescendant)
			continue;

		// move 
		if (control == Control::position)
			transform.SetPosition2D(transform.Position2D() + delta);
		else if (control == Control::rotation)
			transform.SetRotation2D(transform.Rotation2D() + delta.x);
		else if (control == Control::scale)
			transform.SetScale2D(transform.Scale2D() + delta);
	}

}


/*
void SelectionMover::SetPosition(vec2 newPos)
{
	for (const auto& entityPtr : Selector::Selection())
		entityPtr->Get<Transform>().SetPosition2D(newPos);
}

void SelectionMover::SetRotation(float angle)
{
	for (const auto& entityPtr : Selector::Selection())
		entityPtr->Get<Transform>().SetRotation2D(angle);
}

void SelectionMover::SetScale(vec2 newScale)
{
	for (const auto& entityPtr : Selector::Selection())
		entityPtr->Get<Transform>().SetScale2D(newScale);
}
*/
