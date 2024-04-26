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
#include "imGuiTools.h" 

using namespace Editor;
Shorts
uuid currentEntityID;
float dragSensitivity = 0.1f;


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
        Renderable& renderable = *static_cast<Renderable*>(&comp);
        // node["mesh"] = mesh.GetID();
        // node["material"] = material.GetID();

        optional<uuid> meshID = Mesh::naming.Show("mesh", renderable.GetMesh().GetID());
        if (meshID)
        {
            Mesh& mesh = Mesh::register_.Get(*meshID);
            renderable.SetMesh(mesh);
            //P(mesh);
        }
        optional<uuid> matID = Material::naming.Show("material", renderable.GetMaterial().GetID());
        if (matID)
        {
            Material& mat = Material::register_.Get(*matID);
            renderable.SetMaterial(mat);
            //P(mat);
        }
        /*
        static int item_current = 0;
        Naming& naming = Material::naming;
        ImGui::Combo("names", &item_current, naming.Names());
        string name = naming.Names()[item_current];
        uuid id = naming.at(name);
        Material& mat = Material::register_.Get(id);
        */

        /*
        static char str0[128] = "Hello, world!";
        ImGui::InputText("input text 1", str0, IM_ARRAYSIZE(str0));

        const char* items[] = { "name 1", "name 2", "name 3" };
        static int item_current = 0;
        ImGui::Combo("names", &item_current, items, IM_ARRAYSIZE(items)); // used in material, mesh and evt. entity

        static float col2[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
        ImGui::ColorEdit4("color 1", col2); // used in uniform
        int item = 3;
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "WARNING");
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "ERROR");
        */
    }
    else if (typeid(comp) == typeid(PolygonCollider))
    {
        PolygonCollider& poly = *static_cast<PolygonCollider*>(&comp);

        vector<vec2> positions = poly.bare.GetLocalPosition2Ds();
        ImGui::Text("positions");
        for (int i=0; i<(int)positions.size(); i++)
            ImGui::DragFloat2(std::to_string(i).c_str(), glm::value_ptr(positions[i]), dragSensitivity);
        poly.Setup(positions);

    }
    else if (typeid(comp) == typeid(RectangleCollider))
    {
        RectangleCollider& rect = *static_cast<RectangleCollider*>(&comp);
        
        vec2 size = rect.Size();
        ImGui::DragFloat2("size", glm::value_ptr(size), dragSensitivity, 0.001f, 1000.0f);
        rect.SetSize(size);
    }
    else if (typeid(comp) == typeid(CircleCollider))
    {
        CircleCollider& circle = *static_cast<CircleCollider*>(&comp);
     
        float radius = circle.bare.GetLocalRadius();
        ImGui::DragFloat("radius", &radius, dragSensitivity);
        circle.Setup(radius);
    }

}
