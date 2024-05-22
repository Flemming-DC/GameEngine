#include "Collider.h"
#include "ListTools.h"
#include "ErrorChecker.h"

Shorts;
vector<uuid> Collider::allColliders = {};



void Collider::OnStart()
{
	allColliders.push_back(GetID());
}

void Collider::OnDestroy()
{
	bool wasThere = Tools::Remove(allColliders, GetID());
	if (!wasThere)
		RaiseError("ColliderID has been removed from allColliders prematurely");
}

ITransform Collider::MakeTransformInterface(glm::vec2 center)
{
	return { 
		[this, center]() { return glm::ToVec3(center) + GetTransform().GetPosition(); },
		[this]() { return GetTransform().GetScale(); },
		[this](vec2 vec, bool isPos) { return GetTransform().ToWorldSpace(vec, isPos); },
	};
}
