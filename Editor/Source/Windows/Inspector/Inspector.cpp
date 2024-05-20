#include "Inspector.h"
#include "ErrorChecker.h"
#include "Selector.h"
#include "Entity.h"
#include "imGuiTools.h" 
#include "ComponentDrawer.h"

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
        DrawEntityHeader();

        Entity& entity = Entity::GetEntity(currentEntityID);
        for (const unique_ptr<Component>& compPtr : entity.GetComponents())
            DrawComponent(*compPtr);

        AttachComponent();
    }
    ImGui::End();
}

void Inspector::DrawEntityHeader()
{
    static string entityName;
    static bool isEditingName = false;
    if (!isEditingName)
        entityName = Entity::GetEntity(currentEntityID).Name(); // starting choice
    if (ImGui::InputText("name", &entityName))
    {
        if (entityName != "")
        {
            P("");
            isEditingName = false;
        }
        else
            Warning("Entity name mustn't be blank.");
    }
    ImGui::Separator();
    ImGui::NewLine();
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

    ImGui::Indent();
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
    ImGui::Unindent();

    // super dirty way to check if it has exposed parameters. 
    // fails if you choose to support such exposure for custom components.
    bool hasExposedParameters = 
           typeid(comp) == typeid(Transform)
        || typeid(comp) == typeid(Renderable)
        || typeid(comp) == typeid(PolygonCollider)
        || typeid(comp) == typeid(RectangleCollider)
        || typeid(comp) == typeid(CircleCollider);
    if (hasExposedParameters)
        ImGui::Spacing();
}


void Inspector::AttachComponent()
{

    ImGui::NewLine();
    ImGui::SeparatorText("Add Component");
    static string compName;
    vector<string> completionOptions = Tools::GetKeys(Entity::AddComponentByName);
    Tools::Remove(completionOptions, Tools::TypeName<Transform>());
    if (ImGui::AutoCompletor(" ", &compName, completionOptions))
    {
        if (!Tools::ContainsKey(Entity::AddComponentByName, compName))
            RaiseError("Unrecognized component ", compName);
        Entity::AddComponentByName.at(compName)(currentEntityID, nullptr);
        compName.clear();
    }

}

