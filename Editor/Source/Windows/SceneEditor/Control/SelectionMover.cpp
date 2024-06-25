#include "SelectionMover.h"
#include "Selector.h"
#include "Transform.h"
#include "EditorInputs.h"
#include "SceneCamera.h"
#include "Time_.h"
#include "EngineMode.h"

Shorts;
using namespace Editor;

enum class Control { position, rotation, scale, uniformScale };
static Control control = Control::position;
static vec2 lastMouseWorldPosition2D = vec2();
static float moveSpeed = 1.5f; // evt. multiply by zoom (get the zoom from camera, not input)
static float wasDragging = false;
static float startAngle = 0;
static vec2 startScale = vec2(1.0f);

void SelectionMover::Update()
{
	UpdateControl();
	vec2 delta = Delta();
	if (!EngineMode::GameIsRunning())
		Move(delta);

	lastMouseWorldPosition2D = SceneCamera::MouseWorldPosition2D();
	wasDragging = Selector::IsDraggingSelection();
}

void SelectionMover::UpdateControl()
{
	if (EditorInputs::ControlPosition())
		control = Control::position;
	else if (EditorInputs::ControlRotation())
		control = Control::rotation;
	else if (EditorInputs::ControlScale())
		control = Control::scale;
	else if (EditorInputs::ControlUniformScale())
		control = Control::uniformScale;
	
}

vec2 SelectionMover::Delta()
{
	bool isDragging = Selector::IsDraggingSelection();
	vec2 delta;

	vec2 mouseWorldPosition2D = SceneCamera::MouseWorldPosition2D();
	if (isDragging)
		delta = (mouseWorldPosition2D - lastMouseWorldPosition2D);
	else
		delta = EditorInputs::SelectionMoveDirection() * moveSpeed * Time::Delta();
	return delta;
}

void SelectionMover::Move(vec2 delta)
{
	bool isDragging = Selector::IsDraggingSelection();
	for (const uuid& entityID : Selector::Selection())
	{
		Transform& transform = Entity::GetEntity(entityID).Get<Transform>();

		// skip descendants, since they will already be moving due to moving their parents
		bool isDescendant = false;
		for (const uuid& otherID : Selector::Selection())
		{
			isDescendant = transform.IsDescendantOf(Entity::GetEntity(otherID).Get<Transform>());
			if (isDescendant)
				break;
		}
		if (isDescendant)
			continue;

		// move 
		if (control == Control::position)
			transform.SetPosition2D(transform.Position2D() + delta);
		else if (control == Control::rotation && !isDragging)
			transform.SetAngle(transform.Angle() + delta.x);
		else if (control == Control::scale && !isDragging)
			transform.SetScale2D(transform.Scale2D() + delta);
		else if (control == Control::uniformScale && !isDragging)
			transform.SetScale2D(transform.Scale2D() + delta.x);
		else if (control == Control::rotation && isDragging)
		{
			vec2 originalDisplacement = (vec2)Selector::SelectionStartPosition() - transform.Position2D();
			vec2 displacement = SceneCamera::MouseWorldPosition2D() - transform.Position2D();
			float angle = glm::OrientedAngle(displacement, originalDisplacement);
			if (!wasDragging)
				startAngle = transform.Angle();
			transform.SetAngle(startAngle - angle);
		}
		else if (control == Control::scale && isDragging)
		{
			vec2 startDisplacement = (vec2)Selector::SelectionStartPosition() - transform.Position2D(); // dragStartPosition
			vec2 currentDisplacement = SceneCamera::MouseWorldPosition2D() - transform.Position2D();
			float min = std::pow(10.0f, -5.0f);
			if (glm::LessThan(startDisplacement, min))
				startDisplacement = min * glm::sign(startDisplacement);
			vec2 scaling = currentDisplacement / startDisplacement;
			if (!wasDragging)
				startScale = transform.Scale2D();
			transform.SetScale2D(startScale * scaling);
		}
		else if (control == Control::uniformScale && isDragging)
		{
			float startDistance = glm::distance((vec2)Selector::SelectionStartPosition(), transform.Position2D()); // dragStartPosition
			float currentDistance = glm::distance(SceneCamera::MouseWorldPosition2D(), transform.Position2D());
			float min = std::pow(10.0f, -5.0f);
			float scaling = currentDistance / std::max(startDistance, min);
			if (!wasDragging)
				startScale = transform.Scale2D();
			transform.SetScale2D(startScale * scaling );
		}
		else
			RaiseError("unrecognized value ", control);
		
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
