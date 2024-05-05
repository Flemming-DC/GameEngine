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
    if (Entity::Exists(currentEntityID))
    {
        Entity& entity = Entity::GetEntity(currentEntityID);
        for (const unique_ptr<Component>& compPtr : entity.GetComponents())
            DrawComponent(*compPtr);
    }
    
    // attach component
    ImGui::Spacing();
    ImGui::Indent();
    if (ImGui::Button("Add Component"))
    {
        ImGui::Text("clicked");
        P(Entity::AddComponentByName.size());
        for (const auto& [name, func] : Entity::AddComponentByName)
        {
            P(name);
        }
        //P(Entity::AddComponentByName);
    }
    static int selected_fish = -1;
    const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
    if (ImGui::Button("Select.."))
        ImGui::OpenPopup("my_select_popup");
    ImGui::SameLine();
    ImGui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
    if (ImGui::BeginPopup("my_select_popup"))
    {
        ImGui::SeparatorText("Aquarium");
        for (int i = 0; i < IM_ARRAYSIZE(names); i++)
            if (ImGui::Selectable(names[i]))
                selected_fish = i;
        ImGui::EndPopup();
    }
    ImGui::Unindent();

    static string compName;
    vector<string> completionOptions = {"hejsa", "hello", "bim", "hip", "hap", "hup"};
    ImGui::AutoCompletor("Completion custom", &compName, completionOptions);

    
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



