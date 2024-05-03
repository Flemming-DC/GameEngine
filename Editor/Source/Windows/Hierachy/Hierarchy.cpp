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
        //P(1, " ", tr, " ", tr.GetParent());
        if (tr.GetParent() == nullptr)
        {
            //P(2, " ", tr);
            roots.push_back(&tr);
        }
    }
    return roots;
}

void Hierarchy::DrawTreeNode(const Transform& transform)
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


