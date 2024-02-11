#include "BroadPhase.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

// We begin with the trivial broadphase, which returns all pairs, Then we optimize afterwards
Overlaps BroadPhase::GetPotentialOverlaps()
{
	Overlaps allPairs;

	auto colliders = Collider::GetAllColliders();
	for (int i=0; i<(int)colliders.size(); i++)
	{
		for (int j=0; j<i; j++)
			allPairs.push_back({ colliders[i], colliders[j] });
	}
	return allPairs;
}