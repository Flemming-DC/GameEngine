#pragma once
#include "Collider.h"
#include "CollisionLoop.h"
#include <vector>

// this class exposes some functions to be used by the gamelogic, 
// in contrast to CollisionChecker, which is an internal engine only class.

// evt. add GetCircleOverlaps, GetRectangleOverlaps and GetPolygonOverlaps. 
// Either accept dublication or with hidden/engine-only colliders
class CollisionChecker
{
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
public:
	static Overlaps GetOverlaps() { return CollisionLoop::GetOverlaps(); }
	static Collider* RayCast(glm::vec2 startPosition, glm::vec2 direction, float distance = INFINITY);
	static std::vector<Collider*> RayOverlaps(glm::vec2 startPosition, glm::vec2 direction, float distance = INFINITY);
	static std::vector<Collider*> RectangleOverlaps(glm::vec2 upperLeft, glm::vec2 lowerRight);
};

