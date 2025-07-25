#include "ComponentDrawer.h"
#include "imGuiTools.h" 
#include "GlmCheck.h"
#include "EngineAssets.h"
#include "EngineLiterals.h"
#include "EngineMode.h"
#include <glm/gtc/type_ptr.hpp> 

Shorts
using namespace Editor;
float dragSensitivity = 0.003f;

//void DrawCamera(Camera& camera) {}
void ComponentDrawer::DrawTransform(Transform& transform)
{
    if (EngineMode::GameIsRunning())
        ImGui::BeginDisabled();

    vec3 pos = transform.LocalPosition();
    if (ImGui::DragFloat2("position", glm::value_ptr(pos), dragSensitivity))
        transform.SetLocalPosition(pos);

    float angle = transform.LocalAngle();
    if (ImGui::SliderFloat("rotation", &angle, 0.0f, 2.0f * 3.14159265359f))
        transform.SetLocalAngle(angle);

    vec3 scale = transform.LocalScale();
    bool scaleChanged = false;
    if (transform.requireUniformScale)
        scaleChanged = ImGui::DragFloat("scale", &scale.x, 0.5f * dragSensitivity, 0.0001f);
    else
        scaleChanged = ImGui::DragFloat2("scale", glm::value_ptr(scale), dragSensitivity, 0.0001f);
    if (scaleChanged)
    {
        scale.z = 1;
        if (transform.requireUniformScale)
            scale = vec3(scale.x); // this is also done in SetLocalScale, but for clarity we do it here too.
        scale = vec3(Realistic_nz(scale.x), Realistic_nz(scale.y), Realistic_nz(scale.z));
        transform.SetLocalScale(scale);
    }

    ImGui::SameLine();
    ImGui::Checkbox("uniform ", &transform.requireUniformScale);
    
    bool isStatic = transform.IsStatic();
    if (ImGui::Checkbox("static", &isStatic))
        transform.SetStatic(isStatic);
    
    if (EngineMode::GameIsRunning())
        ImGui::EndDisabled();
}

void ComponentDrawer::DrawRenderable(Renderable& renderable)
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
    ImGui::Indent();
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
            mat.SetTexture(Literals::u_textureSampler, *changedTexID);
    }
    ImGui::Unindent();


    int drawOrder = renderable.DrawOrder();
    ImGui::InputInt("drawOrder", &drawOrder);
    renderable.SetDrawOrder(drawOrder);
}

void ComponentDrawer::DrawPolygonCollider(PolygonCollider& poly)
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

void ComponentDrawer::DrawRectangleCollider(RectangleCollider& rect)
{
    vec2 size = rect.Size();
    ImGui::DragFloat2("size", glm::value_ptr(size), dragSensitivity);

    vec2 center = rect.Center();
    ImGui::DragFloat2("center", glm::value_ptr(center), dragSensitivity);

    rect.SetupRect(size, center);
}

void ComponentDrawer::DrawCircleCollider(CircleCollider& circle)
{
    float radius = circle.bare.GetLocalRadius();
    ImGui::DragFloat("radius", &radius, 0.5f * dragSensitivity, 0.001f, 1000.0f);

    vec2 center = circle.Center();
    ImGui::DragFloat2("##center", glm::value_ptr(center), dragSensitivity);

    circle.Setup(Realistic_p(radius), center);
}





