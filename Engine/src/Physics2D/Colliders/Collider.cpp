#include "Collider.h"
#include "ListTools.h"
#include "ErrorChecker.h"


std::vector<Collider*> Collider::allColliders = {};



void Collider::OnStart()
{
	allColliders.push_back(this);
}

void Collider::OnDestroyed()
{
	Tools::Remove(allColliders, this);
	//delete gizmo;
}

