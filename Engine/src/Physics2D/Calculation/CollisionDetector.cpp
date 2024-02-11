#include "CollisionDetector.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"
#include "Collider.h"
#include "ListTools.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
Overlaps CollisionDetector::overlaps = {};

void CollisionDetector::Update()
{
	// potentialCollisions are stored as collider pairs, rather than collision objects
	auto potentialNewOverlaps = BroadPhase::GetPotentialOverlaps();
	auto newOverlaps = NarrowPhase::GetOverlaps(potentialNewOverlaps);
	HandleCollisionInfo(newOverlaps);
	//solve collisions

}


void CollisionDetector::HandleCollisionInfo(Overlaps newOverlaps)
{
	for (const auto& pair : newOverlaps)
	{
		if (!Tools::Contains(overlaps, pair))
		{
			pair.first->onEnter.Invoke(pair.second);
			pair.second->onEnter.Invoke(pair.first);
		}
	}
	for (const auto& pair : overlaps)
	{
		if (!Tools::Contains(newOverlaps, pair))
		{
			pair.first->onExit.Invoke(pair.second);
			pair.second->onExit.Invoke(pair.first);
		}
	}
	overlaps = newOverlaps;
}


std::vector<Collider*> CollisionDetector::RayOverlaps(
	glm::vec2 startPosition, glm::vec2 direction, float distance)
{
	return NarrowPhase::RayOverlaps(startPosition, direction, distance);
}
Collider* CollisionDetector::RayCast(
	glm::vec2 startPosition, glm::vec2 direction, float distance)
{
	return NarrowPhase::RayCast(startPosition, direction, distance);
}

