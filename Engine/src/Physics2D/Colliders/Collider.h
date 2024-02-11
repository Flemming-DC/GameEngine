#pragma once
#include "Component.h"
#include "Transform.h"
#include "Collision.h"
#include "Gizmo.h"
#include <vector>

//struct Bounds { float x, y; };

class Collider : public Component
{
public:
	// add and remove collider to list of all colliders

	static std::vector<Collider*>& GetAllColliders() { return allColliders; }
	//Bounds GetBounds() { return bounds; }

protected:
	Gizmo* gizmo;
private:
	static std::vector<Collider*> allColliders;
	//Bounds bounds;

	void OnConstructed() override;
	void OnDestroyed() override;
};

