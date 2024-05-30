#include "Inspector.h"
#include "ErrorChecker.h"
#include "Selector.h"
#include "Entity.h"
#include "imGuiTools.h" 
#include "ComponentDrawer.h"
#include "StoredEntity.h"

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
    Entity& entity = Entity::GetEntity(currentEntityID);
    if (!isEditingName)
        entityName = entity.Name(); // starting choice
    if (ImGui::InputText("name", &entityName, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (entityName != "")
        {
            entity.SetName(entityName);
            isEditingName = false;
        }
        else
            Warning("Entity name mustn't be blank.");
    }
    if (entity.GetStoredID().has_value())
        ImGui::LabelText("storedEntity", StoredEntity::naming.at(entity.GetStoredID().value()).c_str());
    ImGui::Separator();
    ImGui::NewLine();
}

void Inspector::DrawComponent(Component& comp)
{
    // header
    ImGui::PushID(uuids::to_string(comp.GetID()).c_str());
    bool open = ImGui::CollapsingHeader(Tools::TypeName(comp).c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::PopID();
    if (typeid(comp) != typeid(Transform) && ImGui::BeginPopupContextItem()) // remove comp option
    {
        if (comp.Entity().GetStoredID().has_value())
            ImGui::Text("Cannot Remove Component on stored entity in editor.");
        else if (ImGui::Button("Remove Component"))
        {
            comp.Entity().Destroy(comp);
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    if (!open)
        return;

    bool enabled = comp.Enabled();
    ImGui::PushID((uuids::to_string(comp.GetID()) + "Enabled").c_str());
    ImGui::Checkbox("Enabled", &enabled);
    ImGui::PopID();
    comp.SetEnabled(enabled);

    // exposed fields
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

