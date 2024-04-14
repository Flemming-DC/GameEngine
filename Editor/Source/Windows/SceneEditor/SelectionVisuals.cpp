#include "SelectionVisuals.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "RectangleCollider.h"
#include "Selector.h"

using namespace Editor;


void SelectionVisuals::DrawSelectionBox(glm::vec2 selectionStartPosition, glm::vec2 selectionEndPosition)
{

}

void SelectionVisuals::DrawSelection()
{
    /*
    for (const Collider* col : Selector::Selection())
    {
        bool isCircle1 = (typeid(col) == typeid(CircleCollider));
        bool isPoly1 = (typeid(col) == typeid(PolygonCollider));
        bool isRect= (typeid(col) == typeid(RectangleCollider));

        
    }*/
}


/*
void SelectionVisuals::DrawRect(ImVec2 topLeft, ImVec2 bottomRight, ImU32 color)
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

void DrawPolygon(const ImVec2* points, int numPoints, ImU32 color)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    for (int i = 0; i < numPoints; i++)
    {
        int j = (i + 1) % numPoints; // Next vertex index (wrapping around for the last vertex)
        drawList->AddLine(points[i], points[j], color);
    }
}
*/