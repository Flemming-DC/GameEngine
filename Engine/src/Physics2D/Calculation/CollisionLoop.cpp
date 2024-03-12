#include "CollisionLoop.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"
#include "Collider.h"
#include "ListTools.h"
#include "Scene.h"

using Overlaps = std::vector<std::pair<Collider*, Collider*>>;
Overlaps CollisionLoop::overlaps = {};
auto& _ = Dynamic::Add<CollisionLoop>();

void CollisionLoop::OnEngineStart()
{
	Scene::onEnd.Add([](const Scene& _) { overlaps.clear(); });
}

void CollisionLoop::Update()
{
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
				RaiseError("Collider is null");
			pair.first->onEnter.Invoke(*pair.second); // first enters second
			pair.second->onEnter.Invoke(*pair.first); // second enters first
		}
	}
	for (const auto& pair : overlaps) // exit
	{
		if (!Tools::Contains(newOverlaps, pair))
		{
			if (pair.first == nullptr || pair.second == nullptr)
				RaiseError("Collider is null");
			pair.first->onExit.Invoke(*pair.second); // first exits second
			pair.second->onExit.Invoke(*pair.first); // second exits first
		}
	}
	overlaps = newOverlaps;
}

