#pragma once
#include "Component.h"
#include "Transform.h"
#include "Event.h"
#include "BareCollider.h"
#include <vector>
#include <stduuid/uuid.h>

//struct Bounds { float x, y; };


class Collider : public Component
{
public:
	Event<Collider&> onEnter; // invoked  by the collision detection system
	Event<Collider&> onExit; // invoked  by the collision detection system

	static std::vector<uuids::uuid>& GetAllColliders() { return allColliders; }
	virtual std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const { return Bare().ShadowAlongNormal(normal); };
	//Bounds GetBounds() { return bounds; }
	ITransform MakeTransformInterface();
	virtual const BareCollider& Bare() const = 0;



protected:
	uuids::uuid gizmoID;

private:
	static std::vector<uuids::uuid> allColliders;

	void OnStart() override;
	void OnDestroy() override;
};

