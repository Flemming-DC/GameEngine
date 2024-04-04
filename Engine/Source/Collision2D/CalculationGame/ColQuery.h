#pragma once
#include "Collider.h"
#include "CollisionLoop.h"
#include "ShortHands.h"
#include "ColMaker.h" // when the gamelogic includes ColQuery, then it also needs ColMaker
#include <vector>

// this class exposes some functions to be used by the gamelogic, 
// in contrast to ColQuery, which is an internal engine only class.

// evt. add GetCircleOverlaps, GetRectangleOverlaps and GetPolygonOverlaps. 
// Either accept dublication or with hidden/engine-only colliders

class ColQuery // coliquery, colimata, colisor, ColliQuery, ColQuery
{
	Shorts;
	using Overlaps = vector<pair<Collider*, Collider*>>;
public:
	static Collider* RayCast(vec2 startPosition, vec2 direction, float distance = INFINITY);
	static vector<Collider*> RayOverlaps(vec2 startPosition, vec2 direction, float distance = INFINITY);
	static Overlaps GetAllOverlaps() { return CollisionLoop::GetOverlaps(); }
	static Collider* TryGetOverlap(const BareCollider& collider); // evt.
	static vector<Collider*> GetOverlaps(const BareCollider& collider);
	static bool IsOverlapping(const BareCollider& col1, const BareCollider& col2);
	
};

