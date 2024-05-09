#include "Hierarchy.h"
#include "ImGuiTools.h"
#include "Entity.h"
#include <string>
using namespace Editor;
Shorts;


void Hierarchy::Update()
{
    ImGui::Begin("Hierarchy");

    for (const Transform* tr : FindRoots())
        DrawTreeNode(*tr);


    ImGui::End();
}


vector<Transform*> Hierarchy::FindRoots()
{
    vector<Transform*> roots;
    for (const Entity& entity : Entity::register_.GetData()) // slow loop
    {
        Transform& tr = entity.Get<Transform>();
        if (tr.GetParent() == nullptr)
            roots.push_back(&tr);
    }
    return roots;
}

void Hierarchy::DrawTreeNode(const Transform& transform)
{
    static map_uo<uuid, bool> bib;
    uuid id = transform.GetID();
    if (!Tools::ContainsKey(bib, id))
        bib[id] = false;

    string name = transform.Entity().Name();
    int flag = transform.GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;
    if (bib[id])
        flag |= ImGuiTreeNodeFlags_Selected;
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    bool open = ImGui::TreeNodeEx(name.c_str(), flag);

    if (ImGui::IsItemToggledOpen())
        P("ImGui::IsItemToggledOpen() ", name);
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        P("ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen() ", name);
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
        ImGui::Text("This is a drag and drop source ", name);
        ImGui::EndDragDropSource();
    }


    if (ImGui::IsItemClicked())
    {
        P("clicked ", transform.Entity().Name());
        bib[id] = !bib[id];
    }

    if (open)
    {
        for (const Transform* child : transform.GetChildren())
            DrawTreeNode(*child);
        ImGui::TreePop();
    }



}


void Hierarchy::DrawTreeNode_old(const Transform& transform)
{

    string name = transform.Entity().Name();
    auto flag = transform.GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNodeEx(name.c_str(), flag))
    {
        for (const Transform* child : transform.GetChildren())
            DrawTreeNode(*child);
        ImGui::TreePop();
    }

}
