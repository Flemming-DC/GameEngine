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
#include "GlmCheck.h"
#include "EngineAssets.h"
#include "EngineLiterals.h"

Shorts
using namespace Editor;
uuid currentEntityID;
float dragSensitivity = 0.003f;


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


void DrawTransform(Transform& transform)
{
    ImGui::DragFloat2("position", glm::value_ptr(transform.localPosition), dragSensitivity);

    float angle = transform.LocalAngle();
    ImGui::SliderFloat("rotation", &angle, 0.0f, 2.0f * 3.14159265359f);
    transform.SetLocalAngle(angle);

    vec3 scale = transform.LocalScale();
    if (transform.requireUniformScale)
        ImGui::DragFloat("scale", glm::value_ptr(scale), 0.5f * dragSensitivity);
    else
        ImGui::DragFloat2("scale", glm::value_ptr(scale), dragSensitivity);
    transform.SetLocalScale(scale);

    ImGui::SameLine();
    ImGui::Checkbox("uniform ", &transform.requireUniformScale);

}
//void DrawCamera(Camera& camera) {}
void DrawRenderable(Renderable& renderable)
{
    static uuid lastMeshID, lastMatID;

    optional<uuid> meshID = Mesh::naming.Show("mesh", renderable.GetMesh().GetID());
    if (meshID && *meshID != lastMeshID)
    {
        Mesh& mesh = Mesh::register_.Get(*meshID);
        renderable.SetMesh(mesh);
        lastMeshID = *meshID;
    }
    optional<uuid> matID = Material::naming.Show("material", renderable.GetMaterial().GetID());
    if (matID && *matID != lastMatID)
    {
        Material& mat = Material::register_.Get(*matID);
        renderable.SetMaterial(mat);
        lastMatID = *matID;
    }
    // show uniforms for builtin materials
    Material& mat = renderable.GetMaterial();
    if (mat.GetID() == EngineAssets::DefaultMaterial().GetID())
    {
        vec4 color = mat.GetUniform<vec4>(Literals::u_color);
        ImGui::ColorEdit4(Literals::u_color.c_str(), glm::value_ptr(color));
        mat.SetUniform(Literals::u_color, color);
    }
    else if (mat.GetID() == EngineAssets::ImageMaterial().GetID())
    {
        vec4 color = mat.GetUniform<vec4>(Literals::u_color);
        ImGui::ColorEdit4(Literals::u_color.c_str(), glm::value_ptr(color));
        mat.SetUniform(Literals::u_color, color);
        
        Texture& oldTex = *mat.GetUniform<Texture*>(Literals::u_textureSampler);
        optional<uuid> changedTexID = Texture::naming.Show("texture", oldTex.GetID());
        if (changedTexID && *changedTexID != oldTex.GetID())
            mat.SetTexture(Literals::u_textureSampler, Texture::naming.at(*changedTexID));
    }
}
void DrawPolygonCollider(PolygonCollider& poly)
{
    vector<vec2> positions = poly.bare.GetLocalPosition2Ds();
    ImGui::Text("positions");
    for (int i = 0; i < (int)positions.size(); i++)
    {
        ImGui::DragFloat2(std::to_string(i).c_str(), glm::value_ptr(positions[i]), dragSensitivity);
        poly.SetPosition(i, positions[i]);

        ImGui::SameLine();
        ImGui::PushID(i);
        bool addClicked = ImGui::Button("+");
        ImGui::PopID();
        if (addClicked)
        {
            poly.AddPositionAfter(i);
            break;
        }

        if (positions.size() <= 3)
            continue;
        ImGui::SameLine();
        ImGui::PushID(-1 - i);
        bool removeClicked = ImGui::Button("-");
        ImGui::PopID();
        if (removeClicked)
        {
            poly.RemovePosition(i);
            break;
        }
    }

}
void DrawRectangleCollider(RectangleCollider& rect)
{
    vec2 size = rect.Size();
    ImGui::DragFloat2("size", glm::value_ptr(size), dragSensitivity, 0.001f, 1000.0f);
    rect.SetSize(size);
}
void DrawCircleCollider(CircleCollider& circle)
{
    float radius = circle.bare.GetLocalRadius();
    ImGui::DragFloat("radius", &radius, 0.5f * dragSensitivity);
    circle.Setup(Realistic_p(radius));
}


void Inspector::DrawComponent(Component& comp)
{
    if (!ImGui::CollapsingHeader(Tools::TypeName(comp).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        return;

    if (typeid(comp) == typeid(Transform))
        DrawTransform(*static_cast<Transform*>(&comp));
    else if (typeid(comp) == typeid(Camera))
        ; // no-op
    else if (typeid(comp) == typeid(Renderable))
        DrawRenderable(*static_cast<Renderable*>(&comp));
    else if (typeid(comp) == typeid(PolygonCollider))
        DrawPolygonCollider(*static_cast<PolygonCollider*>(&comp));
    else if (typeid(comp) == typeid(RectangleCollider))
        DrawRectangleCollider(*static_cast<RectangleCollider*>(&comp));
    else if (typeid(comp) == typeid(CircleCollider))
        DrawCircleCollider(*static_cast<CircleCollider*>(&comp));

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