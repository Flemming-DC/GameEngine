#include "EditorLoop.h"
#include "imgui/imgui.h"
#include "Renderer.h"
#include "InputKey.h"
#include "Core.h"
#include "EngineMode.h"
#include "Temp_Inspector.h"
#include "Hierarchy.h"
#include "SceneEditor.h"
#include "OpenGlSetup.h"
#include "Delay.h"

using namespace Editor;


void EditorLoop::OnEditorStart() 
{
    Renderer::ShowWindow(false); 
}


void EditorLoop::OnEditorUpdate()
{
    for (const auto& entity : Entity::register_.GetData())
        Temp_Inspector::TransformGUI2D(entity);

    //Inspector::Draw();
    SceneEditor::Draw();
    //evt. GameView::Draw();
    //AssetFolder::Draw();
    //Hierarchy::Draw();

    //ImGui::ShowDemoWindow();

    if (InputKey::BecomesPressed(Key::Keyboard::R))
    {
        logger::print("R");
        if (EngineMode::GameIsRunning())
            Core::StopRunningGame();
        else
            Core::StartRunningGame();
    }

    if (InputKey::BecomesPressed(Key::Keyboard::exc))
        EngineMode::ExitEditor(); // save first

}


