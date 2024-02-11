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
	static std::vector<Collider*> RayCastOverlaps(
		glm::vec2 startPosition, glm::vec2 direction, float distance = INFINITY);


private:
	// evt. make IsOverLapping public. or make it available from the collider class
	static bool IsOverLapping(Collider* collider1, Collider* collider2);

	static bool IsOverLapping(CircleCollider* cirle1, CircleCollider* cirle2);
	static bool IsOverLapping(CircleCollider* cirle, PolygonCollider* polygon);
	static bool IsOverLapping(PolygonCollider* polygon1, PolygonCollider* polygon2);

};

