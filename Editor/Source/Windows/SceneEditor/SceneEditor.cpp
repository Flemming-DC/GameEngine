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
using namespace Editor;

// make viewport framebuffer size sensitive to my viewport vindow size.

static float moveSpeed = 1.5f;
static const float scrollSpeed = 3.0f;
static const float maxScale = 100;
static glm::vec3 cameraPos = glm::vec3(0.0f);
static const glm::quat cameraRot = glm::NoRotation();
static glm::vec3 cameraScale = glm::vec3(1.0f);



void SceneEditor::Draw()
{
    Selector::Update();
    UpdateCamera();
    UpdateVisuals();
}



void SceneEditor::UpdateCamera()
{
    float scaling = 1 + EditorInputs::Zoom().State() * scrollSpeed * Time::Delta(); // exp(x) = 1 + x + O(x^2) is used
    cameraScale *= scaling;
    cameraScale = glm::clamp(cameraScale, glm::vec3(1.0f) / maxScale, glm::vec3(1.0f) * maxScale);

    glm::vec3 moveDirection = glm::ToVec3(EditorInputs::MoveCamera().State());
    moveSpeed *= scaling;
    cameraPos += moveDirection * moveSpeed * Time::Delta();

}

void SceneEditor::UpdateVisuals()
{
    auto renderResult = Renderer::DrawToFrameBuffer(cameraPos, cameraRot, cameraScale, true);

    ImVec2 uvBottumLeft = { 0, 1 };
    ImVec2 uvTopRight = { 1, 0 };
    ImVec2 size = { (float)renderResult.width, (float)renderResult.height };

    ImGui::Begin("ViewPort");
    ImGui::Image((void*)(intptr_t)renderResult.textureOpenGlid, size, uvBottumLeft, uvTopRight);
    ImGui::End();
}
