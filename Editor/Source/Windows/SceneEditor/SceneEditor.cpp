#include "SceneEditor.h"
#include <string>
#include "imgui/imgui.h"
#include "Renderer.h"
#include "InputKey.h"
#include "Time_.h"
#include "EngineMode.h"
#include "GlmTools.h"
#include "Selector.h"
#include "SceneCamera.h"
#include "SelectionMover.h"
#include "SceneVisuals.h"
#include "EntityMaker.h"


using namespace Editor;

// make viewport framebuffer size sensitive to my viewport vindow size.


void SceneEditor::Update() // should be called update
{
    ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar); // NoMove disables drag & drop

    EntityMaker::Update();
    Selector::Update();
    DrawScene();
    SceneVisuals::Update();
    SceneCamera::UpdateCamera(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    SelectionMover::Update();

    ImGui::End();
}



void SceneEditor::DrawScene()
{
    bool drawGizmos = true;
    auto renderResult = Renderer::DrawToFrameBuffer(
        SceneCamera::Position(), SceneCamera::Rotation(), SceneCamera::Scale(), 
        SceneCamera::MinSceneCorner(), SceneCamera::MaxSceneCorner());

    ImVec2 renderSize = { (float)renderResult.width, (float)renderResult.height };
    ImVec2 windowSize = ImGui::GetWindowSize();
    float scaling = std::max(windowSize.x / renderSize.x, windowSize.y / renderSize.y);
    ImVec2 size = { scaling * renderSize.x, scaling * renderSize.y };

    ImVec2 uvBottumLeft = { 0, 1 };
    ImVec2 uvTopRight = { 1, 0 };

    ImGui::Image((void*)(intptr_t)renderResult.textureOpenGlid, size, uvBottumLeft, uvTopRight);

}
