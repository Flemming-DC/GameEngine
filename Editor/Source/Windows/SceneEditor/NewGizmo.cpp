#include "NewGizmo.h"
#include "Entity.h"
#include "Collider.h"
#include "GeoDrawing.h"

using namespace Editor;
Shorts;
static vec4 gizmoColor = vec4(0.0f, 1.0f, 0.0f, 1.0f); // green
static float lineThickness = 0.2f;
static float gridScale = 0.25f;
static vec4 gridColor = vec4(0.2f, 0.2f, 0.2f, 1);

void NewGizmo::Update()
{
    DrawGrid();

	// evt. restrict to selection and evt. toggle gizmo on and off
	for (const uuid& colID : Collider::GetAllColliders())
	{
		Collider& collider = Entity::GetComponent<Collider>(colID);
		vector<vec2> positions = collider.Bare().Positions();
		GeoDrawing::DrawPolygon(positions, gizmoColor, lineThickness);
	}

}

void NewGizmo::DrawGrid()
{
    float brightness = 0.2f;
    vec4 color = vec4(brightness, brightness, brightness, 1);

    float gridSize = 50;
    vector<vec2> horizontallyOrganizedPosition2Ds;
    vector<vec2> verticallyOrganizedPosition2Ds;
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (float y = -gridSize; y < gridSize; y += gridScale)
    {
        horizontallyOrganizedPosition2Ds.push_back({ -gridSize, y });
        horizontallyOrganizedPosition2Ds.push_back({ gridSize, y });
        ImVec2 point0 = ImVec2(-gridSize, y);
        ImVec2 point1 = ImVec2( gridSize, y);
        drawList->AddLine(point0, point1, ImGui::FromGlm(gizmoColor), lineThickness);
    }
    for (float x = -gridSize; x < gridSize; x += gridScale)
    {
        verticallyOrganizedPosition2Ds.push_back({ x, -gridSize });
        verticallyOrganizedPosition2Ds.push_back({ x,  gridSize });
    }

    GeoDrawing::DrawLines(horizontallyOrganizedPosition2Ds, gridColor, lineThickness);
    GeoDrawing::DrawLines(verticallyOrganizedPosition2Ds, gridColor, lineThickness);

    /*
    horizontalGridID = Gizmo::Make(
        horizontallyOrganizedPosition2Ds, nullptr, color, false, false);
    verticalGridID = Gizmo::Make(
        verticallyOrganizedPosition2Ds, nullptr, color, false, false);
    */
    //GeoDrawing::DrawPolygon(horizontallyOrganizedPosition2Ds, gizmoColor, lineThickness);
}
