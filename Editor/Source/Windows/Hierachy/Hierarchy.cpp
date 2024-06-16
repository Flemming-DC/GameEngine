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
static optional<uuid> renameID = std::nullopt; // id of transform that is currently being renamed

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

    Deny(payload->DataSize <= 0,
        "expected some data from DragDrop");
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
    uuid& id = transform.entity().GetID();
    
    int flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow
        | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
    if (transform.GetChildren().empty())
        flags |= ImGuiTreeNodeFlags_Leaf;
    if (Selector::IsSelected(id))
        flags |= ImGuiTreeNodeFlags_Selected;
    if (renameID == id)
        flags |= ImGuiTreeNodeFlags_AllowOverlap; // allow us to put a text field on top of tree node label


    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    ImGui::PushID(uuids::to_string(id).c_str());
    auto color = transform.entity().GetStoredID().has_value() ? IM_COL32(100, 200, 255, 255) : IM_COL32(255, 255, 255, 255);
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    bool open = ImGui::TreeNodeEx(transform.entity().Name().c_str(), flags);
    ImGui::PopStyleColor();

    Rename(transform);

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
    ImGui::PopID();
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
            Deny(payload->DataSize <= 0,
                "expected some data from DragDrop");
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

void Hierarchy::Rename(Transform& transform)
{
    static string newName;
    uuid& id = transform.entity().GetID();

    if (EditorInputs::Rename() && Selector::IsSelected(id))
    {
        renameID = id;
        newName = transform.entity().Name();
    }
    if (renameID == id)
    {
        ImGui::SameLine();
        ImGui::SetKeyboardFocusHere();
        if (ImGui::InputText(" ", &newName, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            transform.entity().SetName(newName);
            renameID = std::nullopt;
        }
        else if (ImGui::IsItemDeactivated() || ImGui::IsItemDeactivatedAfterEdit())
            renameID = std::nullopt;
    }
}