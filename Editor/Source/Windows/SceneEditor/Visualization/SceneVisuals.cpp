#include "SceneVisuals.h"
#include "SelectionVisuals.h"
#include "Entity.h"
#include "Collider.h"
#include "Gizmo.h"
#include "CircleCollider.h" // temp

using namespace Editor;
Shorts;

void SceneVisuals::Update()
{
    SelectionVisuals::Update();
    DrawGrid();
    DrawColliders();
}

void SceneVisuals::DrawGrid()
{
    vec4 gridColor = vec4(0.2f, 0.2f, 0.2f, 1); // feint grey
    float lineThickness = 0.2f;
    float gridSize = 50;
    float gridScale = 0.25f;

    vector<vec2> horizontallyOrganizedPosition2Ds;
    vector<vec2> verticallyOrganizedPosition2Ds;

    for (float y = -gridSize; y < gridSize; y += gridScale)
    {
        horizontallyOrganizedPosition2Ds.push_back({ -gridSize, y });
        horizontallyOrganizedPosition2Ds.push_back({ gridSize, y });

    }
    for (float x = -gridSize; x < gridSize; x += gridScale)
    {
        verticallyOrganizedPosition2Ds.push_back({ x, -gridSize });
        verticallyOrganizedPosition2Ds.push_back({ x,  gridSize });
    }

    Gizmo::DrawLines(horizontallyOrganizedPosition2Ds, gridColor, lineThickness);
    Gizmo::DrawLines(verticallyOrganizedPosition2Ds, gridColor, lineThickness);

}

void SceneVisuals::DrawColliders()
{
    vec4 colliderColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); // green
    float lineThickness = 0.2f;

    // evt. restrict to selection and evt. toggle gizmo on and off
    for (const uuid& colID : Collider::GetAllColliders())
    {
        Collider& collider = Entity::GetComponent<Collider>(colID);
        vector<vec2> positions = collider.Bare().Positions();
        Gizmo::DrawPolygon(positions, colliderColor, lineThickness);
    }
}