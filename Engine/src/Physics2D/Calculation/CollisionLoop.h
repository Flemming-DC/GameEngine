#pragma once
#include "Collider.h"
#include "glm/glm.hpp"
#include "Dynamic.h"
#include <vector>

// this in an internal engine class, in contrast to CollisionChecker, which is only used by the gameLogic.
class CollisionLoop : public Dynamic
{
	using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

public:
	static void Update();
	static Overlaps GetOverlaps() { return overlaps; }
	static void OnSceneEnd() { overlaps.clear(); }

private:
	static Overlaps overlaps;

	static void HandleCollisionInfo(Overlaps newOverlaps);
	void OnEngineStart() override;
};

