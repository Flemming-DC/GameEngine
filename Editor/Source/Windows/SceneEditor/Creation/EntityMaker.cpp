#include "EntityMaker.h"
#include "ImGuiTools.h"
#include "SceneCamera.h"
#include "StoredEntity.h"
#include "Transform.h"
#include "InputKey.h" // temp

Shorts;
using namespace Editor;
static const char* CreationMenuName = "CreationMenuName";

void EntityMaker::Update()
{
    if (!ImGui::IsWindowHovered() && !ImGui::IsPopupOpen(CreationMenuName))
        return;
 
    CreationMenu();
}




void EntityMaker::CreationMenu()
{
    if (InputKey::BecomesPressed(Key::Mouse::right)) 
        ImGui::OpenPopup(CreationMenuName);
    if (ImGui::BeginPopup(CreationMenuName))
    {
        static string entityName;
        vector<string> completionOptions = StoredEntity::naming.Names(); 
        if (ImGui::AutoCompletor("Choose entity", &entityName, completionOptions))
        {
            if (!Tools::Contains(completionOptions, entityName))
                RaiseError("Unrecognized entity ", entityName);
            P("---------------------------");
            P("chose ", entityName);
            vec2 pos = SceneCamera::MouseWorldPosition2D();
            StoredEntity::Load(entityName).Get<Transform>().SetPosition2D(pos);
            entityName.clear();
        }
        ImGui::EndPopup();
    }
}