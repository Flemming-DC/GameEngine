#include "BroadPhase.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;


Overlaps BroadPhase::GetPotentialOverlaps()
{
	ProfileFunc;
	Overlaps candidates; // candidates for overlap

	ProfileLine(auto colliderIDs = Collider::GetAllColliders(););
	for (int i=0; i<(int)colliderIDs.size(); i++)
	{
		ProfileLine(auto& col1 = Entity::GetComponent<Collider>(colliderIDs[i]););
		ProfileLine(if (!col1.IsFullyEnabled())
			continue;);
		ProfileLine(auto bounds1 = col1.Bare().GetBoundingBox(););

		for (int j = 0; j < i; j++)
		{
			ProfileLine(auto& col2 = Entity::GetComponent<Collider>(colliderIDs[j]););
			ProfileLine(if (!col2.IsFullyEnabled())
				continue;);
			ProfileLine(auto bounds2 = col2.Bare().GetBoundingBox(););

			ProfileLine(if (!bounds1.IsOverlapping(bounds2))
				continue;);
			ProfileLine(candidates.push_back({ &col1, &col2 }););
		}
	}

	if (candidates.size() > 2)
		P("potentialNewOverlapCount: ", candidates.size(), " out of ", Collider::GetAllColliders().size());

	return candidates;
}

