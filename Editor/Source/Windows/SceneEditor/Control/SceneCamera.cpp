#include "SceneCamera.h"
#include "EditorInputs.h"
#include "Screen.h"
using namespace Editor;
Shorts;

static float moveSpeed = 4.5f;
static const float scrollSpeed = 3.0f;
static const float maxScale = 100;
static vec3 cameraPos = vec3(0.0f);
static const quat cameraRot = glm::NoRotation();
static vec3 cameraScale = vec3(3.0f);
static vec2 minSceneCorner;
static vec2 maxSceneCorner;
static vec3 lastMouseWorldPosition = vec3();

void SceneCamera::UpdateCamera(ImVec2 minSceneCorner_, ImVec2 maxSceneCorner_)
{
    // we use ImGui::IsMouseDragging here, because it has to fit with ImGui's builtin 
    // docker dragging mechanism. As such it is independent of our choice of EditorInputs.
    if (!ImGui::IsWindowFocused() && !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        return;
    minSceneCorner = { minSceneCorner_.x, minSceneCorner_.y };
    maxSceneCorner = { maxSceneCorner_.x, maxSceneCorner_.y };

    float scalingDelta = -EditorInputs::Zoom().State() * scrollSpeed * Time::Delta(); 
    float scaling = ImGui::IsWindowHovered() ? 1 + scalingDelta : 1; // exp(x) = 1 + x + O(x^2) is used
    cameraScale *= scaling;
    cameraScale = glm::clamp(cameraScale, vec3(1.0f) / maxScale, vec3(1.0f) * maxScale);

    if (EditorInputs::DragCamera().IsPressed())
    {
        vec3 mousePos = SceneCamera::MouseWorldPosition();
        cameraPos -= (mousePos - lastMouseWorldPosition);
    }
    else //if (ImGui::IsWindowFocused() || ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        vec3 moveDirection = glm::ToVec3(EditorInputs::MoveCamera());
        moveSpeed *= scaling;
        cameraPos += moveDirection * moveSpeed * Time::Delta();
    }
    lastMouseWorldPosition = SceneCamera::MouseWorldPosition();
}

vec3 SceneCamera::MouseWorldPosition()
{
    auto screenPos = EditorInputs::ScreenPosition().State();
    auto editorWorldPos = Screen::ToWorldPosition(
        screenPos, minSceneCorner, maxSceneCorner, 
        cameraPos, cameraRot, cameraScale);
    return editorWorldPos;
}

vec2 SceneCamera::ToWorldPosition(vec2 screenPosition)
{
    return (vec2)Screen::ToWorldPosition(
        screenPosition, minSceneCorner, maxSceneCorner,
        cameraPos, cameraRot, cameraScale);
}
vec2 SceneCamera::FromWorldPosition(vec2 worldPosition)
{
    return Screen::FromWorldPosition(
        glm::ToVec3(worldPosition), minSceneCorner, maxSceneCorner,
        cameraPos, cameraRot, cameraScale);
}


vec3 SceneCamera::Position() { return cameraPos; }
quat SceneCamera::Rotation() { return cameraRot; }
vec3 SceneCamera::Scale() { return cameraScale; }

