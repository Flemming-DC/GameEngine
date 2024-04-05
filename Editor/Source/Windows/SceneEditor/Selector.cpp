#include "Selector.h"
#include "Collider.h"
#include "ShortHands.h"
#include "EditorInputs.h"
#include "ColQuery.h"

using namespace Editor;
Shorts
// evt. add some kind of filter
// static pair<vec2, vec2> selectionBox;
static float minSelectionBoxSize = glm::pow(10.0f, -4.0f);
static vector<Collider*> selection; // entity or collider? // rename // use entityID
static vec2 selectionStartPosition;
static bool isBoxSelecting = false;
//vector<Collider*> Selector::selection; // entity or collider? // rename // use entityID
Event<vector<Collider*>> Selector::onSelected;

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
    P(1);
}

void Selector::UpdateSelectionBox()
{
    vec2 mousePosition = EditorInputs::SelectionPosition().State();
    if (glm::LargerThan(mousePosition - selectionStartPosition, minSelectionBoxSize))
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
        BoxSelect();
    else
        ClickSelect();
    isBoxSelecting = false;
    onSelected.Invoke(selection);
    P(selection);
}


void Selector::BoxSelect()
{
    P(1);
    if (!EditorInputs::KeepSelection().IsPressed())
        selection.clear();

    vec2 selectionEndPosition = EditorInputs::SelectionPosition().State();
    vec2 center = (selectionStartPosition + selectionEndPosition) / 2.0f;
    vec2 size = (selectionStartPosition - selectionEndPosition);
    size = glm::max(size, vec2(minSelectionBoxSize));

    auto colliders = ColQuery::Overlaps(ColMaker::Rectangle(center, size));
    for (const auto& col : colliders)
        selection.push_back(col);
    P(selectionEndPosition, center, size, colliders);
}

void Selector::ClickSelect()
{
    vec2 clickPostion = EditorInputs::SelectionPosition().State();
    P(clickPostion);
    Collider* col = ColQuery::TryGetOverlap(ColMaker::Point(clickPostion));
    P(col);

    if (!col)
        return;
    if (!EditorInputs::KeepSelection().IsPressed())
        selection.clear();
    if (Tools::Contains(selection, col))
        return;
    selection.push_back(col);
}





