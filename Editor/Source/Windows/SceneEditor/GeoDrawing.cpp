#include "GeoDrawing.h"
#include "SceneCamera.h"

using namespace Editor;


void GeoDrawing::DrawCircle(glm::vec2 center, float radius, glm::vec4 color, float thickness)
{
    ImU32 imColor = ImGui::FromGlm(color);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int segmentCount = 40;
    const float segmentAngle = 2 * 3.14159265359f / segmentCount;
    for (int i = 0; i < segmentCount; i++)
    {
        float angle0 = i * segmentAngle;
        float angle1 = (i + 1) * segmentAngle;
        ImVec2 point0(center.x + radius * cos(angle0), center.y + radius * sin(angle0));
        ImVec2 point1(center.x + radius * cos(angle1), center.y + radius * sin(angle1));
        drawList->AddLine(point0, point1, imColor, thickness);
    }

}

void GeoDrawing::DrawRectangle(glm::vec2 center, glm::vec2 size, glm::vec4 color, float thickness)
{
    ImU32 imColor = ImGui::FromGlm(color);
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 bottomLeft = ImVec2(center.x - size.x, center.y - size.y);
    ImVec2 topRight = ImVec2(center.x + size.x, center.y + size.y);
    ImVec2 topLeft = ImVec2(bottomLeft.x, topRight.y);
    ImVec2 bottomRight = ImVec2(topRight.x, bottomLeft.y);

    drawList->AddLine(bottomLeft, bottomRight, imColor, thickness);
    drawList->AddLine(bottomRight, topRight, imColor, thickness);
    drawList->AddLine(topRight, topLeft, imColor, thickness);
    drawList->AddLine(topLeft, bottomLeft, imColor, thickness);
    /*
    drawList->AddLine(topLeft, ImVec2(bottomRight.x, topLeft.y), imColor);
    drawList->AddLine(ImVec2(bottomRight.x, topLeft.y), bottomRight, imColor);
    drawList->AddLine(bottomRight, ImVec2(topLeft.x, bottomRight.y), imColor);
    drawList->AddLine(ImVec2(topLeft.x, bottomRight.y), topLeft, imColor);    
    */
}


void GeoDrawing::DrawPolygon(std::vector<glm::vec2> positions, glm::vec4 color, float thickness)
{
    ImU32 imColor = ImGui::FromGlm(color);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int numPoints = positions.size();

    std::vector<glm::vec2> screenPositions;
    for (const auto pos : positions)
        screenPositions.push_back(SceneCamera::FromWorldPosition(pos));

    for (int i = 0; i < numPoints; i++)
    {
        int j = (i + 1) % numPoints; // Next vertex index (wrapping around for the last vertex)
        drawList->AddLine(ImGui::FromGlm(screenPositions[i]), ImGui::FromGlm(screenPositions[j]), imColor, thickness);
    }
}
