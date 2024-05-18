#include "EditorCore.h"
#include "EngineMode.h"
#include "Dynamic.h"
#include "imgui/imgui.h"
#include "Renderer.h"
#include "EditorInputs.h"
#include "Core.h"
#include "Hierarchy.h"
#include "SceneEditor.h"
#include "Selector.h"
#include "Inspector.h"
#include "MenuBar.h"
using namespace Editor;


void EditorCore::MarkAsEditor()
{
	EngineMode::MarkAsEditor();
	Dynamic::Setup<EditorCore>();
}


void EditorCore::OnEditorStart()
{
    Renderer::ShowWindow(false);
    Selector::Start(); // if other scene classes need some startup, then collect it into one startup function
    Inspector::Start();
}


void EditorCore::OnEditorUpdate()
{
    SceneEditor::Update();
    Inspector::Update();
    Hierarchy::Update();
    //AssetFolder::Update();
    //Console::Update();
    MenuBar::Update();


    HandleOpenClose();
}

void EditorCore::HandleOpenClose()
{
    if (EditorInputs::ToggleRuntime())
    {
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

