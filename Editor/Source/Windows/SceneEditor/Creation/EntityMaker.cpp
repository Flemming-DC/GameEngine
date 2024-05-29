#include "EntityMaker.h"
#include "ImGuiTools.h"
#include "SceneCamera.h"
#include "StoredEntity.h"
#include "Transform.h"
#include "Selector.h"
#include "EditorInputs.h"
#include "Scene.h" 

Shorts;
using namespace Editor;
static const char* PopupCreateEntity = "PopupCreateEntity";
static const char* PopupSaveEntity = "PopupSaveEntity";


void EntityMaker::Update()
{
    if (!ImGui::IsWindowHovered() && !ImGui::IsPopupOpen(PopupCreateEntity) && !ImGui::IsPopupOpen(PopupSaveEntity))
        return;

    Saving();
    CreationMenu();
    Dublication();
    Deletion();
}


void EntityMaker::Saving() // given user input: save a single selected entity to file (i.e. make it stored)
{
    if (EditorInputs::SaveStoredEntity())
    {
        if (Selector::Selection().size() > 1)
            Warning("You can only store one entity at a time, not ", Selector::Selection().size());
        else if (Selector::Selection().size() == 0)
            Warning("You cannot store the selected entity, since you haven't selected any.");
        else
            ImGui::OpenPopup(PopupSaveEntity);
    }
    if (ImGui::BeginPopup(PopupSaveEntity))
    {
        static string name;
        static bool isEditingName = false;
        Entity& entity = Entity::GetEntity(Selector::Selection()[0]);
        if (!isEditingName)
            name = entity.Name(); // starting choice
        if (ImGui::InputText("stored entity name ", &name))
            isEditingName = true;
        if (!isEditingName)
            ImGui::SetItemDefaultFocus();
        if (ImGui::Button("save"))
        {
            if (name != "")
            {
                Scene::Save();
                StoredEntity::Save(entity, name);
                uuid storedID = StoredEntity::naming.at(name);
                entity.SetStoredID(storedID);
                Scene::ReloadImmediately();

                ImGui::CloseCurrentPopup();
                logger::print("saved entity ", name, " to file");
                isEditingName = false;
            }
            else
                Warning("Entity name mustn't be blank.");
        }
        ImGui::EndPopup();
    }
}

void EntityMaker::CreationMenu() // given user input: show popup and create entity from storage
{
    static ImVec2 mousePosition;
    if (EditorInputs::CreateStoredEntity())
    {
        mousePosition = ImGui::GetMousePos();
        ImGui::OpenPopup(PopupCreateEntity);
    }
    ImGui::SetNextWindowPos(mousePosition);
    if (ImGui::BeginPopup(PopupCreateEntity))
    {
        static string entityName;
        vector<string> completionOptions = StoredEntity::naming.Names();
        if (ImGui::AutoCompletor("Choose entity", &entityName, completionOptions, true))
        {
            if (!Tools::Contains(completionOptions, entityName))
                RaiseError("Unrecognized entity ", entityName);
            vec2 pos = SceneCamera::MouseWorldPosition2D();
            StoredEntity::Load(entityName).Get<Transform>().SetPosition2D(pos);
            entityName.clear();
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void EntityMaker::Dublication() // given user input: dublicate selection
{
    if (!EditorInputs::DublicateSelection())
        return;
    for (const uuid& id : Selector::Selection())
    {
        Entity& entityIn = Entity::GetEntity(id);
        Node node = StoredEntity::ToNode(entityIn);
        Entity& entityOut = StoredEntity::FromNode(node, UuidCreator::MakeID(), std::nullopt, true, true);
    }
}

void EntityMaker::Deletion() // given user input: delete selection
{
    if (!EditorInputs::DeleteSelection())
        return;
    for (const uuid& id : Selector::Selection())
        Entity::GetEntity(id).Destroy();
}