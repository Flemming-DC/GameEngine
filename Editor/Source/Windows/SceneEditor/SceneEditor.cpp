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

using namespace Editor;

// make viewport framebuffer size sensitive to my viewport vindow size.


void SceneEditor::Draw()
{
    Selector::Update();
    SceneCamera::UpdateCamera();
    UpdateVisuals();
}



void SceneEditor::UpdateVisuals()
{
    bool drawGizmos = true;
    auto renderResult = Renderer::DrawToFrameBuffer(
        SceneCamera::Position(), SceneCamera::Rotation(), SceneCamera::Scale(), drawGizmos);

    ImVec2 uvBottumLeft = { 0, 1 };
    ImVec2 uvTopRight = { 1, 0 };
    ImVec2 size = { (float)renderResult.width, (float)renderResult.height };

    //ImGui::Begin("ViewPort");
    ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_NoMove); // NoMove disables drag & drop
    ImGui::Image((void*)(intptr_t)renderResult.textureOpenGlid, size, uvBottumLeft, uvTopRight);
    SceneCamera::SetScreenPosition(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    ImGui::End();
}
