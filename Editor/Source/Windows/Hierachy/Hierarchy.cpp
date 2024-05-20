#include "Hierarchy.h"
#include "ImGuiTools.h"
#include "EditorInputs.h"
#include "Selector.h"
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
        DrawTreeNode(*tr);
    DropToRoot();

    ImGui::End();
}


vector<Transform*> Hierarchy::FindRoots()
{
    vector<Transform*> roots;
    for (const Entity& entity : Entity::register_.GetData()) // slow loop
    {
        Transform& tr = entity.Get<Transform>();
        if (tr.Parent() == nullptr)
            roots.push_back(&tr);
    }
    return roots;
}    

void Hierarchy::DropToRoot()
{
    if (dropped)
        return;
    if (!ImGui::GetDragDropPayload())
        return;
    if (!EditorInputs::FinishDragDrop())
        return;
    if (!ImGui::IsWindowFocused())
        return;

    const ImGuiPayload* payload = ImGui::GetDragDropPayload();

    if (payload->DataSize <= 0)
        RaiseError("expected some data from DragDrop");
    uuid childID = *static_cast<uuid*>(payload->Data);
    Transform* child = Entity::TryGetComponent<Transform>(childID);
    ImGui::SetDragDropPayload(dragDrop, nullptr, 0); // remove payload
    if (child)
        child->SetParent(nullptr);
    else
        Warning("The id ", childID, " doesnt correspond to any transform. Failed to set parent.");

}

void Hierarchy::DrawTreeNode(Transform& transform)
{
    uuid id = transform.Entity().GetID();
    
    int flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
    if (transform.GetChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (Selector::IsSelected(id))
        flags |= ImGuiTreeNodeFlags_Selected;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    bool open = ImGui::TreeNodeEx(transform.Entity().Name().c_str(), flags);

    if (ImGui::IsItemClicked()) // nb: this line must come between [bool open =...] and [if (open) ...]
        Selector::SelectFromHierachy(id);

    if (DragDrop(transform)) // nb: this line must come between [bool open =...] and [if (open) ...]
        dropped = true;


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

