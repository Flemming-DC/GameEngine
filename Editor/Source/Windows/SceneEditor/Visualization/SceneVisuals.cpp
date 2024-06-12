#include "SceneVisuals.h"
#include "SelectionVisuals.h"
#include "Entity.h"
#include "Collider.h"
#include "Gizmo.h"
#include "Selector.h"
#include "SceneCamera.h"

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
    float gridScale = 0.5f;

    vec2 UpperLeft = SceneCamera::FromWorldPosition(vec2(-gridSize, -gridSize)); 
    vec2 LowerRight = SceneCamera::FromWorldPosition(vec2(gridSize, gridSize));
    vec2 minScreenPos = vec2(UpperLeft.x, LowerRight.y);
    vec2 maxScreenPos = vec2(LowerRight.x, UpperLeft.y);
    float count = 2 * gridSize / gridScale;
    vec2 gridScreenSize = maxScreenPos - minScreenPos;
    vec2 gridScreenScale = gridScreenSize / count;

    vector<vec2> horizontalScreenPos;
    vector<vec2> verticallyScreenPos;
    for (float y = minScreenPos.y; y < maxScreenPos.y; y += gridScreenScale.y)
    {
        horizontalScreenPos.push_back({ minScreenPos.x, y });
        horizontalScreenPos.push_back({ maxScreenPos.x, y });
    }
    for (float x = minScreenPos.x; x < maxScreenPos.x; x += gridScreenScale.x)
    {
        verticallyScreenPos.push_back({ x, minScreenPos.y });
        verticallyScreenPos.push_back({ x, maxScreenPos.y });
    }

    Gizmo::DrawLines(horizontalScreenPos, gridColor, lineThickness, false);
    Gizmo::DrawLines(verticallyScreenPos, gridColor, lineThickness, false);
}

void SceneVisuals::DrawColliders()
{
    vec4 colliderColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); // green
    float lineThickness = 0.2f;

    // evt. restrict to selection and evt. toggle gizmo on and off
    for (const uuid& colID : Collider::GetAllColliders())
    {
        Collider& collider = Entity::GetComponent<Collider>(colID);
        if (!collider.IsFullyEnabled())
            continue;
        if (!Selector::IsSelected(collider.entity().GetID()))
            continue;
        vector<vec2> positions = collider.Bare().Positions();
        Gizmo::DrawPolygon(positions, colliderColor, lineThickness);
    }
}