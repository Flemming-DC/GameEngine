#pragma once
#include "Collider.h"
#include <vector>

class BroadPhase
{
public:
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

	static Overlaps& GetPotentialOverlaps();

};

