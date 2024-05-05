#include "Inspector.h"
#include "ErrorChecker.h"
#include "Selector.h"
#include "Entity.h"
#include "imGuiTools.h" 
#include "ComponentDrawer.h"
#include "InputKey.h" // temp

Shorts
using namespace Editor;
uuid currentEntityID;

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
    if (!Entity::Exists(currentEntityID))
    {
        ImGui::End();
        return;
    }

    // draw components
    Entity& entity = Entity::GetEntity(currentEntityID);
    for (const unique_ptr<Component>& compPtr : entity.GetComponents())
        DrawComponent(*compPtr);
    
    // attach component
    ImGui::Spacing();
    ImGui::Text("Add Component");
    //static char compName[256];
    string compName;
    //compName.reserve(1000);
    vector<string> completionOptions = Tools::GetKeys(Entity::AddComponentByName);
    Tools::Remove(completionOptions, Tools::TypeName<Transform>());
    //P(currentEntityID);
    if (ImGui::AutoCompletor("Completion custom", &compName, completionOptions))
    {
        
        P("chosen ", compName);
        //string compNameStr = compName;
        if (!Tools::ContainsKey(Entity::AddComponentByName, compName))
            RaiseError("Unrecognized component ", compName);
        Entity::AddComponentByName.at(compName)(currentEntityID);

        for (int i = 0; i < (int)compName.size(); i++)
        {
            compName[i] = '\0';
        }
        compName.clear();// = "";
        ImGui::SetKeyboardFocusHere(-1);
        //compName = "\0";
        // compName.shrink_to_fit();
        //strncpy(compName.c_str(), "", 2048);

        //compName
        //std::memset(compName, '\0', sizeof(compName)); // Clear the char array
        //ImGui::SetKeyboardFocusHere(-1);
    }
    
    
    ImGui::End();
}

void Inspector::DrawComponent(Component& comp)
{
    bool open = ImGui::CollapsingHeader(Tools::TypeName(comp).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    if (typeid(comp) != typeid(Transform) && ImGui::BeginPopupContextItem())
    {
        if (ImGui::Button("Remove Component"))
            comp.Entity().Destroy(comp);
        ImGui::EndPopup();
    }
    if (!open)
        return;


    // -----------
    /*
    if (typeid(comp) == typeid(Transform))
    {
        Transform& tr = *static_cast<Transform*>(&comp);
        string cStr = comp.to_string();
        string trStr = tr.to_string();
        P(cStr, " ", trStr);
    }
    DownCast(ComponentDrawer::Draw(comp));

    if (typeid(comp) == typeid(Transform))
        ComponentDrawer::Draw(*static_cast<Transform*>(&comp));
    else if ...
    else
        RaiseError("unrecognized componentType");

    //ComponentDrawer::Draw(DownCast(comp));
    */
    // -----------


    
    if (typeid(comp) == typeid(Transform))
        ComponentDrawer::DrawTransform(*static_cast<Transform*>(&comp));
    else if (typeid(comp) == typeid(Camera))
        ; // no-op
    else if (typeid(comp) == typeid(Renderable))
        ComponentDrawer::DrawRenderable(*static_cast<Renderable*>(&comp));
    else if (typeid(comp) == typeid(PolygonCollider))
        ComponentDrawer::DrawPolygonCollider(*static_cast<PolygonCollider*>(&comp));
    else if (typeid(comp) == typeid(RectangleCollider))
        ComponentDrawer::DrawRectangleCollider(*static_cast<RectangleCollider*>(&comp));
    else if (typeid(comp) == typeid(CircleCollider))
        ComponentDrawer::DrawCircleCollider(*static_cast<CircleCollider*>(&comp));

}



