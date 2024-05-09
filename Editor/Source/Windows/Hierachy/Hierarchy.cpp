#include "Hierarchy.h"
#include "ImGuiTools.h"
#include "EditorInputs.h"
#include "Entity.h"
#include <string>
using namespace Editor;
Shorts;
static const char* dragDrop = "change parent";
static bool dropped = false; // gets updated by DragDrop inside DrawTreeNode and gets used by DropToRoot

void Hierarchy::Update()
{
    ImGui::Begin("Hierarchy");

    dropped = false;
    for (Transform* tr : FindRoots())
        DrawTreeNode(*tr); // Transform isn't const, since it can change parent
    
    DropToRoot();

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

void Hierarchy::DropToRoot()
{
    if (!dropped && ImGui::GetDragDropPayload() && EditorInputs::FinishDragDrop())
    {
        const ImGuiPayload* payload = ImGui::GetDragDropPayload();

        if (payload->DataSize <= 0)
            RaiseError("expected some data from DragDrop");
        uuid childID = *static_cast<uuid*>(payload->Data);
        Transform& child = Entity::GetComponent<Transform>(childID);
        child.SetParent(nullptr);

        ImGui::SetDragDropPayload(dragDrop, nullptr, 0); // remove payload
    }
}

void Hierarchy::DrawTreeNode(Transform& transform)
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


    if (DragDrop(transform))
        dropped = true;

    if (ImGui::IsItemClicked())
    {
        bib[id] = !bib[id];
    }

    if (open)
    {
        for (Transform* child : transform.GetChildren())
            DrawTreeNode(*child);
        ImGui::TreePop();
    }



}

bool Hierarchy::DragDrop(Transform& transform)
{
    if (ImGui::BeginDragDropSource())
    {
        uuid transformID = transform.GetID();
        ImGui::SetDragDropPayload(dragDrop, &transformID, sizeof(transformID));
        ImGui::EndDragDropSource();
    }


    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dragDrop))
        {
            if (payload->DataSize <= 0)
                RaiseError("expected some data from DragDrop");
            uuid childID = *static_cast<uuid*>(payload->Data);
            Transform& child = Entity::GetComponent<Transform>(childID);
            child.SetParent(&transform);
        }
        ImGui::EndDragDropTarget();
        return true;
    }
    return false;
    /*
    else if (const ImGuiPayload* payload = ImGui::GetDragDropPayload())
    {
        //const ImGuiPayload* payload = ImGui::GetDragDropPayload();
        P(payload != nullptr);
    }
    */

}


void Hierarchy::DrawTreeNode_old(const Transform& transform)
{

    string name = transform.Entity().Name();
    auto flag = transform.GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None;
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNodeEx(name.c_str(), flag))
    {
        for (Transform* child : transform.GetChildren())
            DrawTreeNode(*child);
        ImGui::TreePop();
    }

}
