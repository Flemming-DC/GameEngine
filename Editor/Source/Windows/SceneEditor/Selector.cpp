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
static vector<Entity*> selection;
static vec2 selectionStartPosition;
static bool isBoxSelecting = false;
static int clickSelectionIndex;
Event<vector<Entity*>> Selector::onSelected;

void Selector::Update()
{
    if (EditorInputs::Select().BecomesPressed())
        StartSelecting();
    else if (EditorInputs::Select().BecomesReleased())
        FinishSelecting();
    else if (EditorInputs::Select().IsPressed())
        UpdateSelectionBox();
}


void Selector::StartSelecting()
{
    selectionStartPosition = SceneCamera::MouseWorldPosition();
    UpdateSelectionBox();
}

void Selector::UpdateSelectionBox()
{
    vec2 mousePosition = SceneCamera::MouseWorldPosition();
    if (glm::LargerThan(mousePosition - selectionStartPosition, minSelectionBoxSize))
        isBoxSelecting = true;
}

void Selector::FinishSelecting()
{
    if (isBoxSelecting)
        BoxSelect();
    else
        ClickSelect();
    isBoxSelecting = false;
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
    for (const auto& entity : overlaps)
        selection.push_back(entity);
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
    if (Tools::Contains(selection, clickedEntity))
        return;
    selection.push_back(clickedEntity);
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
            vec2 pos = (vec2)transform.GetPosition();
            if (ColQuery::IsOverlapping(selectionRect, ColMaker::Point(pos)))
                overlaps.push_back(&entity);
        }
    }
    return overlaps;
}


std::vector<Entity*> Selector::Selection() { return selection; }

glm::vec2 Selector::SelectionStartPosition() { return selectionStartPosition; }
