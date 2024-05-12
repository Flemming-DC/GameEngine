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
    if (!ImGui::Begin("MenuBar", &open, ImGuiWindowFlags_MenuBar))
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
        New();
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Open", "Ctrl+O"))
    {
        Open();
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S"))
        Scene::Save();

    ImGui::EndMenu();
}


void MenuBar::New()
{
    static string name;
    static string errorMessage;
    static bool editted = false;
    if (ImGui::InputText("##", &name))
        editted = true;

    if (name == "" && editted)
        errorMessage = "Scene name mustn't be blank.";
    else if (Scene::naming.Contains(name))
        errorMessage = "Scene name is already used.";
    else
        errorMessage = "";

    ImGui::SameLine();
    if (ImGui::Button("Create Scene"))
    {
        if (errorMessage == "" && editted)
        {
            Scene::MakeBlankSceneFile(name); // only makes the yml, not the subclass.
            ImGui::CloseCurrentPopup();
            logger::print("Create Scene ", name);
            editted = false;
        }
        else
            editted = true;
    }
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), errorMessage.c_str());
}

void MenuBar::Open()
{
    static string sceneName;
    if (ImGui::AutoCompletor("Choose scene", &sceneName, Scene::naming.Names()))
    {
        if (!Tools::Contains(Scene::naming.Names(), sceneName))
            RaiseError("Unrecognized scene ", sceneName);
        Scene::Save();
        Scene::Activate(sceneName);
        sceneName.clear();
        ImGui::CloseCurrentPopup();
    }
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
