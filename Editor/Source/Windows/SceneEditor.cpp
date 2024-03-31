#include "SceneEditor.h"
#include <string>
#include "imgui/imgui.h"
#include "Renderer.h"
#include "InputKey.h"
#include "Time_.h"
#include "EngineMode.h"

// make viewport framebuffer size sensitive to my viewport vindow size.

static glm::vec3 cameraPos = glm::vec3(0.0f);
static const glm::quat cameraRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
static glm::vec3 cameraScale = glm::vec3(1.0f);
static float moveSpeed = 1.0f; // camera
static const float scrollSpeed = 4.0f / 60.0f; // camera
static const float maxScale = 100;


void SceneEditor::Draw()
{
    UpdateCamera();

    auto renderResult = Renderer::DrawToFrameBuffer(cameraPos, cameraRot, cameraScale);

    ImVec2 uvBottumLeft = { 0, 1 };
    ImVec2 uvTopRight = { 1, 0 };
    ImVec2 size = { (float)renderResult.width, (float)renderResult.height };

    ImGui::Begin("ViewPort");
    ImGui::Image((void*)(intptr_t)renderResult.textureOpenGlid, size, uvBottumLeft, uvTopRight);
    ImGui::End();
}



void SceneEditor::UpdateCamera()
{
    // we dont multiply by delta_time, since mouseScrollDelta already acculumates over time.
    float scaling = 1 + InputKey::GetFloat(Key::FloatKey::mouseScrollDelta) * scrollSpeed; // exp(x) = 1 + x + O(x^2) is used
    cameraScale *= scaling;
    cameraScale = glm::clamp(cameraScale, glm::vec3(1.0f) / maxScale, glm::vec3(1.0f) * maxScale);

    glm::vec3 moveDirection = glm::vec3(0.0f);
    if (InputKey::IsPressed(Key::Keyboard::A))
        moveDirection.x -= 1;
    if (InputKey::IsPressed(Key::Keyboard::D))
        moveDirection.x += 1;
    if (InputKey::IsPressed(Key::Keyboard::S))
        moveDirection.y -= 1;
    if (InputKey::IsPressed(Key::Keyboard::W))
        moveDirection.y += 1;
    if (moveDirection.x != 0 && moveDirection.y != 0)
        moveDirection = glm::normalize(moveDirection);

    moveSpeed *= scaling;
    cameraPos += moveDirection * moveSpeed * Time::Delta();
}


