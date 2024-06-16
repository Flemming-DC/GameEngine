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
	Shorts;
	Event<Collider&> onEnter; // invoked  by the collision detection system
	Event<Collider&> onExit; // invoked  by the collision detection system

	static vector<uuid>& CollidersSortedByX() { return collidersSortedByX; }
	virtual pair<float, float> ShadowAlongNormal(vec2 normal) const { return Bare().ShadowAlongNormal(normal); };
	//Bounds GetBounds() { return bounds; }
	ITransform MakeTransformInterface(vec2 center = vec2(0.0f));
	virtual const BareCollider& Bare() const = 0;



protected:
	virtual void OnStart() override;

private:
	static vector<uuid> collidersSortedByX;

	void OnUpdate() override;
	void OnDestroy() override;
};

