#include "Collider.h"
#include "ListTools.h"
#include "ErrorChecker.h"


std::vector<uuids::uuid> Collider::allColliders = {};



void Collider::OnStart()
{
	allColliders.push_back(GetID());
	Log("Collider.OnStart: allColliders.size " + std::to_string(allColliders.size()));
}

void Collider::OnDestroyed()
{
	bool wasThere = Tools::Remove(allColliders, GetID());
	if (!wasThere)
		RaiseError("ColliderID has been removed from allColliders prematurely");
	Log("Collider.OnDestroyed: allColliders.size " + std::to_string(allColliders.size()));
	/*
	bool wasThere = Tools::Remove(allRenderables, GetID());
    if (!wasThere)
        RaiseError("RenderableID has been removed from allRenderables prematurely");
    Log("Renderable.OnDestroyed: allRenderables.size " + std::to_string(allRenderables.size()));
	*/
	//delete gizmo;
}

