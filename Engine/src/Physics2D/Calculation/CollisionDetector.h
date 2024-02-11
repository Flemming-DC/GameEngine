#pragma once
#include "Collider.h"
#include <vector>


class CollisionDetector
{
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

public:
	static void Update();
	static Overlaps GetOverlaps() { return overlaps; }
private:
	static Overlaps overlaps;

	static void HandleCollisionInfo(Overlaps newOverlaps);
};

