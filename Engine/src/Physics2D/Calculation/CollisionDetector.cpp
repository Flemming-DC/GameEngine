#include "CollisionDetector.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"
#include "Collider.h"
//#include "glm/ext.hpp" 
#include "glm/gtx/string_cast.hpp"
#include "ListTools.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
static Overlaps lastOverlaps = {};

void CollisionDetector::Update()
{
	// potentialCollisions are stored as collider pairs, rather than collision objects
	auto potentialOverlaps = BroadPhase::GetPotentialOverlaps();
	auto overlaps = NarrowPhase::GetOverlaps(potentialOverlaps);
	HandleCollisionInfo(overlaps);
	//solve collisions

}


void CollisionDetector::HandleCollisionInfo(Overlaps overlaps)
{
	for (const auto& pair : overlaps)
	{
		if (!Tools::Contains(lastOverlaps, pair))
		{
			pair.first->onEnter.Invoke(pair.second);
			pair.second->onEnter.Invoke(pair.first);
		}
	}
	for (const auto& pair : lastOverlaps)
	{
		if (!Tools::Contains(overlaps, pair))
		{
			pair.first->onExit.Invoke(pair.second);
			pair.second->onExit.Invoke(pair.first);
		}
	}
	lastOverlaps = overlaps;
}


