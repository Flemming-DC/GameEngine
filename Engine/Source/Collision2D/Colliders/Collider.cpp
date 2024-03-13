#include "Collider.h"
#include "ListTools.h"
#include "ErrorChecker.h"


std::vector<uuids::uuid> Collider::allColliders = {};



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

