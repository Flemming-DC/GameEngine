#include "Collider.h"
#include "ListTools.h"
#include "ErrorChecker.h"
#include <algorithm>

Shorts;
vector<uuid> Collider::collidersSortedByX;



void Collider::OnStart()
{
	Deny(Tools::Contains(collidersSortedByX, GetID()),
		"ColliderID has already been added. id = ", GetID());

	auto iteratorX = Tools::FindIf(collidersSortedByX, [this](uuid id)
		{ return GetTransform().Position2D().x < Entity::GetComponent<Collider>(id).Get<Transform>().Position2D().x; });
	collidersSortedByX.insert(iteratorX, GetID());

}

void Collider::OnDestroy()
{
	bool wasThereX = Tools::Remove(collidersSortedByX, GetID());
	Assert(wasThereX,
		"ColliderID has been removed from collidersSortedByX prematurely");
}

void Collider::OnUpdate() // maintaining sorted collider lists
{
	// looks slow, but doesn't seem to bad accordiing to measurement
	int indexX = Tools::FindIndex(collidersSortedByX, GetID()); 
	if (indexX < (int)collidersSortedByX.size() - 1)
	{
		uuid nextIdX = collidersSortedByX[indexX + 1];
		float nextX = Entity::GetComponent<Collider>(nextIdX).Get<Transform>().Position2D().x;
		float thisX = GetTransform().Position2D().x;
		if (thisX > nextX) 
			std::swap(collidersSortedByX[indexX], collidersSortedByX[indexX + 1]);
	}


}

ITransform Collider::MakeTransformInterface(glm::vec2 center)
{
	return { 
		[this, center]() { return glm::ToVec3(center) + GetTransform().GetPosition(); },
		[this]() { return GetTransform().GetScale(); },
		[this](vec2 vec, bool isPos) { return GetTransform().ToWorldSpace(vec, isPos); },
	};
}
