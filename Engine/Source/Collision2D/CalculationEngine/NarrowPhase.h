#pragma once
#include "Collider.h"
#include "BareCircleCollider.h"
#include "BarePolygonCollider.h"
#include <vector>


class NarrowPhase
{
public:
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
	typedef const BareCircleCollider& circle;
	typedef const BarePolygonCollider& poly;

	static Overlaps GetOverlaps(Overlaps potentialOverlaps);
	static bool IsOverLapping(const BareCollider& collider1, const BareCollider& collider2);

private:
	static bool IsOverLapping_CC(circle cirle1, circle cirle2);
	static bool IsOverLapping_CP(circle cirle, poly polygon);
	static bool IsOverLapping_PP(poly polygon1, poly polygon2);

};

