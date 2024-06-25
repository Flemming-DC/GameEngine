#include "SelectionVisuals.h"
#include "Collider.h"
#include "Renderable.h"
#include "Selector.h"
#include "ImGuiTools.h"
#include "SceneCamera.h"
#include "EditorInputs.h"
#include "imgui/imgui.h"
#include "Gizmo.h"


Shorts;
using namespace Editor;
static float minSize = 0.1f; // min Size of the display of a selected object
static vec4 selectionColor = vec4(0.0f, 0.5f, 1.0f, 0.5f);
static float lineThickness = 5.0f;

void SelectionVisuals::Update()
{
    SelectionVisuals::DrawSelection();
    if (EditorInputs::Select().IsPressed() && Selector::IsSelecting())
        SelectionVisuals::DrawSelectionBox(Selector::SelectionStartPosition(), SceneCamera::MouseWorldPosition());
}


void SelectionVisuals::DrawSelectionBox(glm::vec2 selectionStartPosition, glm::vec2 selectionEndPosition)
{
    if (Selector::IsDraggingSelection())
        return;
    vector<vec2> positions = {
        vec2(selectionStartPosition.x, selectionStartPosition.y),
        vec2(selectionEndPosition.x, selectionStartPosition.y),
        vec2(selectionEndPosition.x, selectionEndPosition.y),
        vec2(selectionStartPosition.x, selectionEndPosition.y),
    };
    Gizmo::DrawPolygon(positions, selectionColor, lineThickness);
}

void SelectionVisuals::DrawSelection()
{
    for (const uuid& entityID : Selector::Selection())
    {
        Entity& entity = Entity::GetEntity(entityID);
        Display(entity);
    }
}

void SelectionVisuals::Display(const Entity& entity)
{
    vector<vec2> positions;
    Renderable* renderable = entity.TryGet<Renderable>();
    Collider* collider = entity.TryGet<Collider>();
    Transform& transform = entity.Get<Transform>();
    
    if (renderable)
    {
        auto localPositions2D = renderable->GetMesh().FindPositions2D();
        for (const vec2 localPos2D : localPositions2D)
            positions.emplace_back(transform.ToWorldSpace(localPos2D, true));
    }
    else if (collider)
    {
        positions = collider->Bare().Positions();
    } 
    else
    {
        float minHalfSize = minSize / 2.0f;
        vec2 pos = (vec2)transform.Position();
        positions = {
            vec2(pos.x - minHalfSize, pos.y - minHalfSize),
            vec2(pos.x + minHalfSize, pos.y - minHalfSize),
            vec2(pos.x + minHalfSize, pos.y + minHalfSize),
            vec2(pos.x - minHalfSize, pos.y + minHalfSize),
        };
    }

    Gizmo::DrawPolygon(positions, selectionColor, lineThickness);
}

