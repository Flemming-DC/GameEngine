#include "EditorSandbox.h"
#include "InputKey.h"
#include "ColQuery.h"
#include "Renderer.h"
#include "Time_.h"
#include "InputVectorizer.h"
#include "Screen.h"
#include "Camera.h"
#include "OpenGlSetup.h"
#include "SceneCamera.h"

using namespace Editor;
using namespace Key;
Shorts;


void TestMousePosition()
{
    auto screenPos = InputVectorizer::GetVectorInput(Key::VectorKey::MouseEntireScreenPosition);
    //auto normScreenPos = InputVectorizer::GetVectorInput(Key::VectorKey::MouseEntireScreenPosition);
    //auto worldPos = InputVectorizer::GetVectorInput(Key::VectorKey::mouseWorldPosition2D);

    //auto back_to_normScreenPos = Screen::FromWorldPosition(glm::ToVec3(worldPos));


    auto editorWorldPos = Screen::ToWorldPosition(screenPos, vec2(674.0f, 146.0f), vec2(1634.599976f, 866.599976f),
        SceneCamera::Position(), SceneCamera::Rotation(), SceneCamera::Scale());
    if (InputKey::BecomesPressed(Keyboard::P))
    {
        logger::print("P");
        logger::print("screenPos: ", screenPos);
        //logger::print("normScreenPos: ", normScreenPos);
        //logger::print("worldPos: ", worldPos);
        //logger::print("back_to_normScreenPos: ", back_to_normScreenPos);
        logger::print("editorWorldPos: ", editorWorldPos);
    }
    if (InputKey::BecomesPressed(Keyboard::O))
        glfwSetWindowPos(OpenGlSetup::GetWindow(), 100, 100);
}


void EditorSandbox::OnEditorStart()
{

}


void EditorSandbox::OnEditorUpdate()
{
    TestMousePosition();
}

