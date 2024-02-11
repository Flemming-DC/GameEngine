#pragma once
#include "Collider.h"
#include "glm/glm.hpp"
#include <vector>


class CollisionDetector
{
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

public:
	static void Update();
	static Overlaps GetOverlaps() { return overlaps; }
	static std::vector<Collider*> RayOverlaps(glm::vec2 startPosition, glm::vec2 direction, float distance = INFINITY);
	static Collider* RayCast(glm::vec2 startPosition, glm::vec2 direction, float distance = INFINITY);
private:
	static Overlaps overlaps;

	static void HandleCollisionInfo(Overlaps newOverlaps);
};

