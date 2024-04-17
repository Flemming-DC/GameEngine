#include "EditorLoop.h"
#include "imgui/imgui.h"
#include "Renderer.h"
#include "InputKey.h"
#include "Core.h"
#include "EngineMode.h"
//#include "Temp_Inspector.h"
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

    ImGui::ShowDemoWindow();

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


