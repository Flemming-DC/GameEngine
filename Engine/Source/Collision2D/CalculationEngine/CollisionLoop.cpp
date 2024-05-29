#include "CollisionLoop.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"
#include "Collider.h"
#include "ListTools.h"
#include "Scene.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
Overlaps CollisionLoop::overlaps = {};


void CollisionLoop::Setup()
{
	Scene::onEnd.Add([](const Scene& _) { overlaps.clear(); });
}

void CollisionLoop::Update()
{
	PruneDeadColliders();
	// potentialCollisions are stored as collider pairs, rather than collision objects
	auto potentialNewOverlaps = BroadPhase::GetPotentialOverlaps();
	auto newOverlaps = NarrowPhase::GetOverlaps(potentialNewOverlaps);
	HandleCollisionInfo(newOverlaps);
	//solve collisions

}


void CollisionLoop::HandleCollisionInfo(Overlaps newOverlaps)
{
	for (const auto& pair : newOverlaps) // enter
	{
		if (!Tools::Contains(overlaps, pair))
		{
			if (pair.first == nullptr || pair.second == nullptr)
				RaiseError("Collider is null ", pair);
			pair.first->onEnter.Invoke(*pair.second); // first enters second

			if (pair.first == nullptr || pair.second == nullptr)
				RaiseError("Collider became null during collision. ", pair);
			pair.second->onEnter.Invoke(*pair.first); // second enters first
		}
	}
	for (const auto& pair : overlaps) // exit
	{
		if (!Tools::Contains(newOverlaps, pair))
		{
			if (pair.first == nullptr || pair.second == nullptr)
				RaiseError("Collider is null ", pair);
			pair.first->onExit.Invoke(*pair.second); // first exits second

			if (pair.first == nullptr || pair.second == nullptr)
				RaiseError("Collider became null during collision. ", pair);
			pair.second->onExit.Invoke(*pair.first); // second exits first
		}
	}
	overlaps = newOverlaps;
}


void CollisionLoop::PruneDeadColliders()
{
	Overlaps toBeRemoved;
	for (const auto& [col1, col2] : overlaps)
	{
		if (!Tools::Contains(Collider::GetAllColliders(), col1->GetID()))
			toBeRemoved.push_back({ col1, col2 });
		else if (!Tools::Contains(Collider::GetAllColliders(), col2->GetID()))
			toBeRemoved.push_back({ col1, col2 });
	}
	for (const auto& pair : toBeRemoved)
		Tools::Remove(overlaps, pair);

}


