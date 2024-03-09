#include "BroadPhase.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

// We begin with the trivial broadphase, which returns all pairs, Then we optimize afterwards
Overlaps BroadPhase::GetPotentialOverlaps()
{
	Overlaps allPairs;

	auto colliderIDs = Collider::GetAllColliders();
	for (int i=0; i<(int)colliderIDs.size(); i++)
	{
		for (int j = 0; j < i; j++)
		{
			auto& col1 = Entity::GetComponent<Collider>(colliderIDs[i]);
			auto& col2 = Entity::GetComponent<Collider>(colliderIDs[i]);
			allPairs.push_back({ &col1, &col2 });
		}
	}
	return allPairs;
}