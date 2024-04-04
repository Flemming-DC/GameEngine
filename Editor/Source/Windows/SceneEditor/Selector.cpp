#include "Selector.h"
#include "Entity.h" // both entity and collider?
#include "Collider.h" // both entity and collider?
#include "ShortHands.h"
#include "EditorInputs.h"
#include "Time_.h"
#include "ColQuery.h"

using namespace Editor;
Shorts

//[SerializeField] LayerMask mask;
static pair<vec2, vec2> selectionBox;
//[SerializeField] float doubleClickDuration = 0.5f; // when I click at my max speed, then I have 0.16 seconds between each click
static float minSelectionBoxSize;
static vector<uuid> selection; // entity or collider? // rename // use entityID
static vec2 selectionStartPosition;
//static Entity* lastClickedUnit;
//float lastClickTime;
static bool isBoxSelecting;

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
    selectionStartPosition = EditorInputs::SelectionPosition().State();
    UpdateSelectionBox();
}

void Selector::UpdateSelectionBox()
{
    vec2 mousePosition = EditorInputs::SelectionPosition().State();

    if (glm::dot(mousePosition, selectionStartPosition))
        isBoxSelecting = true;
    /*
    float boxX = glm::abs(mousePosition.x - selectionStartPosition.x);
    float boxY = glm::abs(mousePosition.y - selectionStartPosition.y);
    
    selectionBox.sizeDelta = new vec2(boxX, boxY);
    vec2 boxCenter = (mousePosition + selectionStartPosition) / 2;
    selectionBox.anchoredPosition = boxCenter;


    */
}


void Selector::FinishSelecting()
{
    if (isBoxSelecting)
    {
        isBoxSelecting = false;
        SelectUnitsInBox();
        return;
    }
    isBoxSelecting = false;
    Entity* entity = TryGetSelectableByClick();
    if (entity)
    {
        SelectSingleUnit(*entity);
        /*
        bool isDoubleClicking = Time::EditorNow() - lastClickTime < doubleClickDuration;
        if (isDoubleClicking && entity == lastClickedUnit)
            SelectUnitType();
        else
            SelectSingleUnit(*entity);

        lastClickTime = Time::EditorNow();
        lastClickedUnit = entity;
        */
    }

}


void Selector::SelectSingleUnit(const Entity& entity)
{
    ClearSelectionUnlessShiftSelecting();
    if (Tools::Contains(selection, entity.GetID()))
        return;
    AddToSelection(entity);
}

/*
void Selector::SelectUnitType()
{
    ClearSelectionUnlessShiftSelecting();
    for (const auto& entity : Entity::register_.GetData())
        if (entity.prefabType == lastClickedUnit.prefabType && !Tools::Contains(selection, entity.GetID()))
            AddToSelection(entity);
    
}
*/

void Selector::SelectUnitsInBox()
{
    ClearSelectionUnlessShiftSelecting();
    // collision detection: find overlaps between selectionBox and entity.bounds

    /*
    vec2 boxMin = selectionBox.anchoredPosition - selectionBox.sizeDelta / 2;
    vec2 boxMax = selectionBox.anchoredPosition + selectionBox.sizeDelta / 2;

    for (const auto& entity : Entity::register_.GetData())
    {
        vec3 halfHorizontalSize = 0.5f * entity.GetComponent<BoxCollider>().size.Horizontal(); // basically the bounds of the unit
        vec2 selectableMin = mainCamera.WorldToScreenPoint(selectable.transform.position - halfHorizontalSize);
        vec2 selectableMax = mainCamera.WorldToScreenPoint(selectable.transform.position + halfHorizontalSize);
        bool isWithinSelectionBox = selectableMax.x > boxMin.x && selectableMin.x < boxMax.x&&
            selectableMax.y > boxMin.y && selectableMin.y < boxMax.y;
        if (!isWithinSelectionBox)
            continue;
        if (Tools::Contains(selection, entity.GetID()))
            continue;
        AddToSelection(entity);
    }
    */
}



void Selector::AddToSelection(const Entity& entity)
{
    selection.push_back(entity.GetID());
    //invoke selection event, to be used by inspector. evt. move that invocation into finishSelection
}



void Selector::ClearSelectionUnlessShiftSelecting()
{
    bool isShiftSelecting = EditorInputs::KeepSelection().IsPressed();
    if (!isShiftSelecting)
    {
        //for (const auto& entityID : selection)
        //    selectedUnit.Deselect();
        selection.clear();
    }
}

Entity* Selector::TryGetSelectableByClick()
{
    // collisionDetection get overlaps with point
    return nullptr;
    /*
    Ray ray = mainCamera.ScreenPointToRay(EditorInputs::SelectionPosition().State());
    selectable = null;


    if (!Physics.Raycast(ray, out RaycastHit hit, Mathf.Infinity, mask))
        return false;
    //else if (!hit.collider.TryGetComponent(out selectable))
    //    return false;
    else
        return true;
    */
}


