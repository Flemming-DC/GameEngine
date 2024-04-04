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
	static Overlaps GetOverlaps() { return CollisionLoop::GetOverlaps(); }
	static Collider* RayCast(vec2 startPosition, vec2 direction, float distance = INFINITY);
	static vector<Collider*> RayOverlaps(vec2 startPosition, vec2 direction, float distance = INFINITY);
	//static std::vector<Collider*> RectangleOverlaps(glm::vec2 upperLeft, glm::vec2 lowerRight);

	// static std::vector<Collider&> GetOverlaps<T>(Args... args); // args of make bare T-collider overload
	/*
	inline static vector<Collider*> GetOverlaps(const Collider& col) { return GetOverlaps(col.Bare()); };
	inline static bool IsOverlapping(const Collider& col1, const Collider& col2)
		{ return IsOverlapping(col1.Bare(), col2.Bare()); };
	*/
	static Collider* TryGetOverlap(const BareCollider& collider); // evt.
	static vector<Collider*> GetOverlaps(const BareCollider& collider);
	static bool IsOverlapping(const BareCollider& col1, const BareCollider& col2);
	
};

