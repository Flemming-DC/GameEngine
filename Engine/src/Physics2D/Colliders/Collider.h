#pragma once
#include "Component.h"
#include "Transform.h"
#include "Gizmo.h"
#include "Event.h"
#include <vector>
#include <stduuid/uuid.h>

//struct Bounds { float x, y; };

class Collider : public Component
{
public:
	Event<Collider&> onEnter; // invoked  by the collision detection system
	Event<Collider&> onExit; // invoked  by the collision detection system

	static std::vector<uuids::uuid>& GetAllColliders() { return allColliders; }
	virtual std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const = 0;
	//Bounds GetBounds() { return bounds; }

protected:
	uuids::uuid gizmoID;
private:
	static std::vector<uuids::uuid> allColliders;
	//std::vector<Collider*> overlaps = {};
	//Bounds bounds;

	void OnStart() override;
	void OnDestroyed() override;
};

