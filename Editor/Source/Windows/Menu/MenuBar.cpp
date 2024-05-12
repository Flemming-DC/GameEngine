#include "MenuBar.h"
#include "ImGuiTools.h"
#include "EditorInputs.h" 
#include "EngineMode.h" 
#include "Scene.h" 
using namespace Editor;
Shorts;

static bool open = true;


void MenuBar::Update()
{
    if (!ImGui::Begin("bim", &open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        // Scene::Save(); popup(er du sikker) ?
        EngineMode::ExitEditor();
        return;
    }
    //ImGui::End();
    if (!ImGui::BeginMenuBar())
        return;

    MenuBar::SceneMenu();
    /*
    if (ImGui::BeginMenu("File"))
    {
        MenuBar::ShowExampleMenuFile();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
    */
    ImGui::EndMainMenuBar();

}

void MenuBar::SceneMenu()
{
    if (!ImGui::BeginMenu("Scenes"))
        return;

    if (ImGui::BeginMenu("New"))
    {
        static string name;
        ImGui::InputText("##", &name);
        ImGui::SameLine();
        if (ImGui::Button("Create Scene"))
        {
            if (name != "")
            {
                Scene::MakeBlankSceneFile(name); // only makes the yml, not the subclass.
                ImGui::CloseCurrentPopup();
                logger::print("Create Scene ", name);
            }
            else
                Warning("Entity name mustn't be blank.");
        }
        ImGui::EndMenu();
    }

    if (ImGui::MenuItem("Open", "Ctrl+O"))
    {
        // evt. save current file first
        // choose from naming via completor
        // Scene::Activate(chosenScene); activate via string is missing
    }

    if (ImGui::MenuItem("Save", "Ctrl+S"))
    {
        Scene::Save();
    }


    ImGui::EndMenu();
}

void _ShowExampleMenuFile()
{
    //ImGui::MenuItem("(dummy menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        ImGui::EndMenu();
    }

}
