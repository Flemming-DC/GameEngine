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
    //ImGui::End(); hvis ImGuiWindowFlags_MenuBar fjernes, så skal End() kaldes her.
    if (!ImGui::BeginMenuBar())
        return;

    MenuBar::SceneMenu();
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
    if (ImGui::BeginMenu("Open"))
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


