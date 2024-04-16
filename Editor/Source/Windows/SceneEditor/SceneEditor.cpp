#include "SceneEditor.h"
#include <string>
#include "imgui/imgui.h"
#include "Renderer.h"
#include "InputKey.h"
#include "Time_.h"
#include "EngineMode.h"
#include "EditorInputs.h"
#include "GlmTools.h"
#include "Selector.h"
#include "SceneCamera.h"
#include "SelectionVisuals.h"
#include "SelectionMover.h"

using namespace Editor;

// make viewport framebuffer size sensitive to my viewport vindow size.


void SceneEditor::Update() // should be called update
{
    Selector::Update();
    SceneCamera::UpdateCamera();
    UpdateVisuals();
    SelectionMover::Update();
}



void SceneEditor::UpdateVisuals()
{
    ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar); // NoMove disables drag & drop

    bool drawGizmos = true;
    auto renderResult = Renderer::DrawToFrameBuffer(
        SceneCamera::Position(), SceneCamera::Rotation(), SceneCamera::Scale(), drawGizmos);

    ImVec2 renderSize = { (float)renderResult.width, (float)renderResult.height };
    ImVec2 windowSize = ImGui::GetWindowSize();
    float scaling = std::max(windowSize.x / renderSize.x, windowSize.y / renderSize.y);
    ImVec2 size = { scaling * renderSize.x, scaling * renderSize.y };

    ImVec2 uvBottumLeft = { 0, 1 };
    ImVec2 uvTopRight = { 1, 0 };

    ImGui::Image((void*)(intptr_t)renderResult.textureOpenGlid, size, uvBottumLeft, uvTopRight);

    SceneCamera::SetScreenBoundary(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    SelectionVisuals::DrawSelection();
    if (EditorInputs::Select().IsPressed())
        SelectionVisuals::DrawSelectionBox(Selector::SelectionStartPosition(), SceneCamera::MouseWorldPosition());
    ImGui::End();
}
