#include "Inspector.h"
#include "ErrorChecker.h"
#include "Selector.h"
#include "Entity.h"
#include "imgui/imgui.h"

using namespace Editor;
Shorts
uuid currentEntityID;
/*
Entity* CurrentEntity()
{
    return Selector::Selection().empty() ? nullptr : &Entity::GetEntity(Selector::Selection()[0]);
}
*/
void Inspector::Start()
{
    Selector::onSelected.Add([](vector<uuid> selection) {
        if (!Selector::Selection().empty()) 
            currentEntityID = Selector::Selection()[0]; 
        });
}

void Inspector::Update()
{
    ImGui::Begin("Inspector");
    if (Entity::Exists(currentEntityID))
    {
        Entity& entity = Entity::GetEntity(currentEntityID);
        for (const unique_ptr<Component>& compPtr : entity.GetComponents())
            DrawComponent(*compPtr);
    }
    ImGui::End();
}


void Inspector::DrawComponent(const Component& comp)
{
    string typeName = Tools::TypeName(comp);
    if (!ImGui::CollapsingHeader(typeName.c_str()))
        return;
    // IsWindowFocused
    //ImGui::Text("var");
    //ImGui::SameLine();
    float floatValue = 0.0f;
    ImGui::InputFloat("Float", &floatValue);

}