#pragma once
#include "Component.h"
#include "Transform.h"
#include "Gizmo.h"
#include "Event.h"
#include <vector>

//struct Bounds { float x, y; };

class Collider : public Component
{
public:
	Event<Collider*> onEnter; // invoked  by the collision detection system
	Event<Collider*> onExit; // invoked  by the collision detection system

	static std::vector<Collider*>& GetAllColliders() { return allColliders; }
	virtual std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const = 0;
	//Bounds GetBounds() { return bounds; }

protected:
	Gizmo gizmo;
	//Gizmo* gizmo;
private:
	static std::vector<Collider*> allColliders;
	//std::vector<Collider*> overlaps = {};
	//Bounds bounds;

	void OnConstructed() override;
	void OnDestroyed() override;
};

