#pragma once
#include "Collider.h"
#include <vector>


class CollisionDetector
{
public:
	static void Update();
private:
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
	static void HandleCollisionInfo(Overlaps overlaps);
};

