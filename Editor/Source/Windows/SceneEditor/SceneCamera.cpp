#include "SceneCamera.h"
#include "EditorInputs.h"
#include "Screen.h"
using namespace Editor;
Shorts;

static float moveSpeed = 1.5f;
static const float scrollSpeed = 3.0f;
static const float maxScale = 100;
static vec3 cameraPos = vec3(0.0f);
static const quat cameraRot = glm::NoRotation();
static vec3 cameraScale = vec3(1.0f);
static vec2 minSceneCorner;
static vec2 maxSceneCorner;


void SceneCamera::UpdateCamera()
{

    float scaling = 1 + EditorInputs::Zoom().State() * scrollSpeed * Time::Delta(); // exp(x) = 1 + x + O(x^2) is used
    cameraScale *= scaling;
    cameraScale = glm::clamp(cameraScale, vec3(1.0f) / maxScale, vec3(1.0f) * maxScale);

    vec3 moveDirection = glm::ToVec3(EditorInputs::MoveCamera().State());
    moveSpeed *= scaling;
    cameraPos += moveDirection * moveSpeed * Time::Delta();

}

vec3 SceneCamera::MouseWorldPosition()
{
    auto screenPos = EditorInputs::ScreenPosition().State();
    auto editorWorldPos = Screen::ToWorldPosition(
        screenPos, minSceneCorner, maxSceneCorner, 
        cameraPos, cameraRot, cameraScale);
    return editorWorldPos;
}

void SceneCamera::SetScreenPosition(ImVec2 minSceneCorner_, ImVec2 maxSceneCorner_)
{
    minSceneCorner = { minSceneCorner_.x, minSceneCorner_.y };
    maxSceneCorner = { maxSceneCorner_.x, maxSceneCorner_.y };
}

vec3 SceneCamera::Position() { return cameraPos; }
quat SceneCamera::Rotation() { return cameraRot; }
vec3 SceneCamera::Scale() { return cameraScale; }

