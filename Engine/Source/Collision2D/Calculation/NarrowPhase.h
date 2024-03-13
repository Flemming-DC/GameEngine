#pragma once
#include "Collider.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#include "PolygonCollider.h"
#include <vector>


class NarrowPhase
{
public:
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

	static Overlaps GetOverlaps(Overlaps potentialOverlaps);

private:
	// evt. make IsOverLapping public. or make it available from the collider class
	static bool IsOverLapping(Collider* collider1, Collider* collider2);

	static bool IsOverLapping_CC(CircleCollider* cirle1, CircleCollider* cirle2);
	static bool IsOverLapping_CP(CircleCollider* cirle, PolygonCollider* polygon);
	static bool IsOverLapping_PP(PolygonCollider* polygon1, PolygonCollider* polygon2);

};

