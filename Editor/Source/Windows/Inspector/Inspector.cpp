#include "Inspector.h"
#include "ErrorChecker.h"
#include "Selector.h"
#include "Entity.h"
#include "imgui/imgui.h"
#include "Transform.h" // evt temp
#include "Camera.h" // evt temp
#include "Renderable.h" // evt temp
#include "PolygonCollider.h" // evt temp
#include "RectangleCollider.h" // evt temp
#include "CircleCollider.h" // evt temp
#include <glm/gtc/type_ptr.hpp> // evt temp


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


void Inspector::DrawComponent(Component& comp)
{
    if (!ImGui::CollapsingHeader(Tools::TypeName(comp).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        return;

    if (typeid(comp) == typeid(Transform))
    {
        Transform& transform = *static_cast<Transform*>(&comp);
        float dragSensitivity = 0.1f;
        ImGui::DragFloat2("position", glm::value_ptr(transform.localPosition), dragSensitivity);

        float angle = transform.LocalAngle();
        ImGui::SliderFloat("rotation", &angle, 0.0f, 2.0f * 3.14159265359f);
        transform.SetLocalAngle(angle);

        vec3 scale = transform.LocalScale();
        if (transform.requireUniformScale)
            ImGui::DragFloat("scale", glm::value_ptr(scale), dragSensitivity);
        else
            ImGui::DragFloat2("scale", glm::value_ptr(scale), dragSensitivity);
        transform.SetLocalScale(scale);
        
        ImGui::SameLine();
        ImGui::Checkbox("uniform ", &transform.requireUniformScale);
    }
    else if (typeid(comp) == typeid(Camera)) {}
    else if (typeid(comp) == typeid(Renderable))
    {
        // node["mesh"] = mesh.GetID();
        // node["material"] = material.GetID();
    }
    else if (typeid(comp) == typeid(PolygonCollider))
    {
        //node["localPosition2Ds"] = bare.GetLocalPosition2Ds();

    }
    else if (typeid(comp) == typeid(RectangleCollider))
    {
        // node["size"] = size; evt. demand that it matches renderer

    }
    else if (typeid(comp) == typeid(CircleCollider))
    {

        // node["localRadius"] = bare.GetLocalRadius();
    }

}
