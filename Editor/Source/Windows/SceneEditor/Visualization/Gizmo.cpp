#include "Gizmo.h"
#include "SceneCamera.h"
#include "logger.h"

using namespace Editor;



void Gizmo::DrawPolygon(std::vector<glm::vec2> positions, glm::vec4 color, float thickness)
{
    positions.push_back(positions[0]);
    DrawLines(positions, color, thickness);
}

void Gizmo::DrawLines(std::vector<glm::vec2> positions, glm::vec4 color, float thickness, bool castPositionsToScreen)
{
    ImU32 imColor = ImGui::FromGlm(color);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    int numPoints = positions.size();

    std::vector<glm::vec2> screenPositions;
    if (castPositionsToScreen)
    {
        for (const auto pos : positions)
            screenPositions.push_back(SceneCamera::FromWorldPosition(pos));
    }
    else
        screenPositions = positions;

    for (int i = 0; i < numPoints - 1; i++)
    {
        int j = (i + 1);//% numPoints; // Next vertex index (wrapping around for the last vertex)
        drawList->AddLine(ImGui::FromGlm(screenPositions[i]), ImGui::FromGlm(screenPositions[j]), imColor, thickness);
    }
}
