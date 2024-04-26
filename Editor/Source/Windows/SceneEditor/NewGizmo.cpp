#include "NewGizmo.h"
#include "Entity.h"
#include "Collider.h"
#include "GeoDrawing.h"

using namespace Editor;
Shorts;
static vec4 gizmoColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); // green
static float lineThickness = 0.2f;

void NewGizmo::Update()
{
	// evt. restrict to selection and evt. toggle gizmo on and off
	for (const uuid& colID : Collider::GetAllColliders())
	{
		Collider& collider = Entity::GetComponent<Collider>(colID);
		vector<vec2> positions = collider.Bare().Positions();
		GeoDrawing::DrawPolygon(positions, gizmoColor, lineThickness);
	}
}
