#include "SelectionVisuals.h"
#include "Collider.h"
#include "Renderable.h"
#include "Selector.h"
#include "ImGuiTools.h"
#include "SceneCamera.h"
#include "imgui/imgui.h"

Shorts;
using namespace Editor;
static float minSize = 0.1f; // min Size of the display of a selected object

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
    DrawPolygon(positions);
}

void SelectionVisuals::DrawSelection()
{
    for (const Entity* entity : Selector::Selection())
    {
        if (!entity)
            RaiseError("entity is nullptr, but that shouldn't be possible.");
        Display(*entity);
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
        vec2 pos = (vec2)transform.GetPosition();
        positions = {
            vec2(pos.x - minHalfSize, pos.y - minHalfSize),
            vec2(pos.x + minHalfSize, pos.y - minHalfSize),
            vec2(pos.x + minHalfSize, pos.y + minHalfSize),
            vec2(pos.x - minHalfSize, pos.y + minHalfSize),
        };
    }

    DrawPolygon(positions);
}

void SelectionVisuals::DrawPolygon(vector<vec2> positions)
{
    ImU32 selectionColor = IM_COL32(0, 125, 255, 125); // blue with alpha = 50%
    float thickness = 5.0f;
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int numPoints = positions.size();

    vector<vec2> screenPositions;
    for (const auto pos : positions)
        screenPositions.push_back(SceneCamera::FromWorldPosition(pos));

    for (int i = 0; i < numPoints; i++)
    {
        int j = (i + 1) % numPoints; // Next vertex index (wrapping around for the last vertex)
        drawList->AddLine(ImGui::FromGlm(screenPositions[i]), ImGui::FromGlm(screenPositions[j]), selectionColor, thickness);
    }
}

/*
void DrawRect(ImVec2 topLeft, ImVec2 bottomRight, ImU32 color)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddLine(topLeft, ImVec2(bottomRight.x, topLeft.y), color);
    drawList->AddLine(ImVec2(bottomRight.x, topLeft.y), bottomRight, color);
    drawList->AddLine(bottomRight, ImVec2(topLeft.x, bottomRight.y), color);
    drawList->AddLine(ImVec2(topLeft.x, bottomRight.y), topLeft, color);
}


void DrawCircle(ImVec2 center, float radius, ImU32 color, int numSegments = 20)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    const float segmentAngle = 2 * 3.14159265359f / numSegments;
    for (int i = 0; i < numSegments; i++)
    {
        float angle0 = i * segmentAngle;
        float angle1 = (i + 1) * segmentAngle;
        ImVec2 point0(center.x + radius * cos(angle0), center.y + radius * sin(angle0));
        ImVec2 point1(center.x + radius * cos(angle1), center.y + radius * sin(angle1));
        drawList->AddLine(point0, point1, color);
    }
}
*/
