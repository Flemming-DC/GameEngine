#include "EditorLoop.h"
#include "imgui/imgui.h"
#include "Renderer.h"
#include "EditorInputs.h"
#include "Core.h"
#include "EngineMode.h"
#include "Hierarchy.h"
#include "SceneEditor.h"
#include "Selector.h"
#include "OpenGlSetup.h"
#include "Inspector.h"
#include "Delay.h"


using namespace Editor;


void EditorLoop::OnEditorStart() 
{
    Renderer::ShowWindow(false); 
    Selector::Start(); // if other scene classes need some startup, then collect it into one startup function
    Inspector::Start();
}


void EditorLoop::OnEditorUpdate()
{
    //for (const auto& entity : Entity::register_.GetData())
    //    Temp_Inspector::TransformGUI2D(entity);

    SceneEditor::Update();
    Inspector::Update();
    //AssetFolder::Update();
    //Hierarchy::Update

    //ImGui::ShowDemoWindow();

    HandleOpenClose();
}

void EditorLoop::HandleOpenClose()
{
    if (EditorInputs::ToggleRuntime())
    {
        logger::print("R");
        if (EngineMode::GameIsRunning())
            Core::StopRunningGame();
        else
            Core::StartRunningGame();
    }

    if (EditorInputs::Save())
        Scene::Save();
    if (EditorInputs::Exit())
    {
        // Scene::Save(); popup(er du sikker) ?
        EngineMode::ExitEditor();
    }
}

