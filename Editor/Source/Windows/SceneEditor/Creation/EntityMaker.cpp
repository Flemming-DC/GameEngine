#include "EntityMaker.h"
#include "ImGuiTools.h"
#include "SceneCamera.h"
#include "StoredEntity.h"
#include "Transform.h"
#include "Selector.h"
#include "EditorInputs.h" 
#include "InputKey.h" // temp

Shorts;
using namespace Editor;
static const char* PopupCreateEntity = "PopupCreateEntity";
static const char* PopupSaveEntity = "PopupSaveEntity";


void EntityMaker::Update()
{
    if (!ImGui::IsWindowHovered() && !ImGui::IsPopupOpen(PopupCreateEntity) && !ImGui::IsPopupOpen(PopupSaveEntity))
        return;

    SaveEntity();
    CreationMenu();
    Dublication();
    Deletion();
}


void EntityMaker::SaveEntity()
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
            StoredEntity::Save(entity, name);
            ImGui::CloseCurrentPopup();
            logger::print("saved entity ", name, " to file");
            isEditingName = false;
        }
        ImGui::EndPopup();
    }
}

void EntityMaker::CreationMenu()
{
    if (EditorInputs::CreateStoredEntity())
        ImGui::OpenPopup(PopupCreateEntity);
    if (ImGui::BeginPopup(PopupCreateEntity))
    {
        static string entityName;
        vector<string> completionOptions = StoredEntity::naming.Names();
        if (ImGui::AutoCompletor("Choose entity", &entityName, completionOptions))
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

void EntityMaker::Dublication()
{
    if (!EditorInputs::DublicateSelection())
        return;
    for (const uuid& id : Selector::Selection())
    {
        Entity& entityIn = Entity::GetEntity(id);
        Node node = StoredEntity::ToNode(entityIn);
        Entity& entityOut = StoredEntity::FromNode(node, UuidCreator::MakeID(), std::nullopt, true);
    }
}

void EntityMaker::Deletion()
{
    if (!EditorInputs::DeleteSelection())
        return;
    for (const uuid& id : Selector::Selection())
        Entity::GetEntity(id).Destroy();
}