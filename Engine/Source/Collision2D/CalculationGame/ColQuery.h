#pragma once
#include "Collider.h"
#include "CollisionLoop.h"
#include "ShortHands.h"
#include "ColMaker.h" // when the gamelogic includes ColQuery, then it also needs ColMaker
#include <vector>


class ColQuery 
{
	Shorts;
	//using Overlaps = vector<pair<Collider*, Collider*>>;
public:
	static Collider* RayCast(vec2 startPosition, vec2 direction, float distance = INFINITY);
	static vector<Collider*> RayOverlaps(vec2 startPosition, vec2 direction, float distance = INFINITY);
	static vector<pair<Collider*, Collider*>> AllOverlaps() { return CollisionLoop::GetOverlaps(); }
	static Collider* TryGetOverlap(const BareCollider& collider); // evt.
	static vector<Collider*> Overlaps(const BareCollider& collider);
	static bool IsOverlapping(const BareCollider& col1, const BareCollider& col2);
	
};

