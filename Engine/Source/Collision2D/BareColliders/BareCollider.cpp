#include "BareCollider.h"
#include "ListTools.h"
#include "ErrorChecker.h"

/*
std::vector<uuids::uuid> BareCollider::allColliders = {};


void BareCollider::OnStart()
{
	allColliders.push_back(GetID());
}

void BareCollider::OnDestroy()
{
	bool wasThere = Tools::Remove(allColliders, GetID());
	if (!wasThere)
		RaiseError("ColliderID has been removed from allColliders prematurely");
}
*/
