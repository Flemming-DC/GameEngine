#include "Selector.h"
#include "Collider.h"
#include "Entity.h"
#include "Renderable.h"
#include "ShortHands.h"
#include "SceneCamera.h"
#include "EditorInputs.h"
#include "ColQuery.h"

using namespace Editor;
Shorts
// evt. add some kind of filter
// static pair<vec2, vec2> selectionBox;
static float minSelectionBoxSize = glm::pow(10.0f, -1.0f);
static float minSize = minSelectionBoxSize; // the min size allows one to select objects without anything to give them a size
static vector<uuid> selection; // entity-uuid's
static vec2 selectionStartPosition;
static bool mouseMoved = false;
static int clickSelectionIndex;
static bool clickedOnSelection = false;
static bool isSelecting;
Event<vector<uuid>> Selector::onSelected; // < entity-uuid's >

void Selector::Start()
{
    Entity::OnDestroy.Add([](Entity& destroyed) { 
        Tools::Remove(selection, destroyed.GetID()); });
}

void Selector::Update()
{
    //if (ImGui::IsWindowHovered()) {}

    if (EditorInputs::Select().BecomesPressed() && ImGui::IsWindowHovered())
        StartSelecting();
    else if (EditorInputs::Select().BecomesReleased() && isSelecting)
        FinishSelecting();
    else if (EditorInputs::Select().IsPressed() && isSelecting)
        UpdateSelectionBox();
}


void Selector::StartSelecting()
{
    isSelecting = true;
    selectionStartPosition = SceneCamera::MouseWorldPosition();
    clickedOnSelection = ClickedOnSelection();
    UpdateSelectionBox();
}

void Selector::UpdateSelectionBox()
{
    vec2 mousePosition = SceneCamera::MouseWorldPosition();
    if (glm::LargerThan(mousePosition - selectionStartPosition, minSelectionBoxSize))
        mouseMoved = true;
}

void Selector::FinishSelecting()
{
    if (mouseMoved && !clickedOnSelection)
        BoxSelect();
    else if (!mouseMoved)
        ClickSelect();
    mouseMoved = false;
    clickedOnSelection = false;
    isSelecting = false;
    if (!IsDraggingSelection())
        onSelected.Invoke(selection);
}


void Selector::BoxSelect()
{
    if (!EditorInputs::KeepSelection().IsPressed())
        selection.clear();

    vec2 selectionEndPosition = SceneCamera::MouseWorldPosition();
    vec2 center = (selectionStartPosition + selectionEndPosition) / 2.0f;
    vec2 size = glm::abs((selectionStartPosition - selectionEndPosition));
    size = glm::max(size, vec2(minSelectionBoxSize));


    auto overlaps = GetOverlaps(center, size);
    //auto colliders = ColQuery::Overlaps(ColMaker::Rectangle(center, size));
    for (const Entity* entity : overlaps)
        selection.push_back(entity->GetID());
}

void Selector::ClickSelect()
{
    vec2 clickPostion = SceneCamera::MouseWorldPosition();
    //Collider* col = ColQuery::TryGetOverlap(ColMaker::Point(clickPostion));
    auto overlaps = GetOverlaps(clickPostion, vec2(minSize));
    if (clickSelectionIndex >= (int)overlaps.size())
        clickSelectionIndex = 0;
    Entity* clickedEntity = overlaps.empty() ? nullptr : overlaps[clickSelectionIndex];
    clickSelectionIndex++;

    if (!EditorInputs::KeepSelection().IsPressed())
        selection.clear();
    if (!clickedEntity)
        return;
    if (Tools::Contains(selection, clickedEntity->GetID()))
        return;
    //Entity& root = clickedEntity->Get<Transform>().Root().entity();
    selection.push_back(clickedEntity->GetID());
}

vector<Entity*> Selector::GetOverlaps(vec2 selectionBoxCenter, vec2 selectionBoxSize)
{
    vector<Entity*> overlaps;
    auto selectionRect = ColMaker::Rectangle(selectionBoxCenter, selectionBoxSize);
    for (auto& entity : Entity::register_.GetData())
    {
        Renderable* renderable = entity.TryGet<Renderable>();
        Collider* collider = entity.TryGet<Collider>();
        Transform& transform = entity.Get<Transform>();
        if (renderable)
        {
            auto localPositions2D = renderable->GetMesh().FindPositions2D();
            vector<vec2> positions;
            for (const vec2 localPos2D : localPositions2D)
                positions.emplace_back(transform.ToWorldSpace(localPos2D, true));
            if (ColQuery::IsOverlapping(selectionRect, ColMaker::Polygon(positions)))
                overlaps.push_back(&entity);
        }
        else if (collider)
        {
            if (ColQuery::IsOverlapping(selectionRect, collider->Bare()))
                overlaps.push_back(&entity);
        }
        else
        {
            vec2 pos = (vec2)transform.Position();
            if (ColQuery::IsOverlapping(selectionRect, ColMaker::Point(pos)))
                overlaps.push_back(&entity);
        }
    }
    return overlaps;
}

bool Selector::ClickedOnSelection()
{
    vector<Entity*> overlaps = GetOverlaps(selectionStartPosition, vec2(minSelectionBoxSize));
    for (const auto& selectedID : selection)
    {
        for (const auto& clicked : overlaps)
        {
            if (clicked->GetID() == selectedID)
                return true;
        }
    }
    return false;
}

bool Selector::IsDraggingSelection() { return clickedOnSelection && mouseMoved; }

vector<uuid> Selector::Selection() { return selection; }

vec2 Selector::SelectionStartPosition() { return selectionStartPosition; }

bool Selector::IsSelecting() { return isSelecting; }


void Selector::SelectFromHierachy(uuid id)
{
    bool wasSelected = IsSelected(id);
    if (!EditorInputs::KeepSelection().IsPressed())
        selection.clear();

    if (wasSelected && EditorInputs::KeepSelection().IsPressed()) // we only toggle, when keeping selection
        Tools::Remove(selection, id);
    else
        selection.push_back(id);
    onSelected.Invoke(selection);
}



bool Selector::IsSelected(uuid id) { return Tools::Contains(selection, id); }
