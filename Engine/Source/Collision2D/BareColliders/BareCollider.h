#pragma once
#include "Component.h"
#include "Transform.h"
#include "Gizmo.h"
#include "Event.h"
#include <vector>
#include <functional>
#include <stduuid/uuid.h>

//struct Bounds { float x, y; };

class BareCollider
{
public:
	//Event<BareCollider&> onEnter; // invoked by the collision detection system
	//Event<BareCollider&> onExit; // invoked by the collision detection system

	//static std::vector<uuids::uuid>& GetAllColliders() { return allColliders; }
	virtual std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const = 0;
	//Bounds GetBounds() { return bounds; }

	std::function<glm::vec3()> ToWorldSpace; //glm::vec3 position, bool isPosition
	std::function<glm::vec3()> Position;
	std::function<glm::vec3()> Scale;

//protected:
	//uuids::uuid gizmoID;

private:
	//static std::vector<uuids::uuid> allColliders;
	//Bounds bounds;

};

