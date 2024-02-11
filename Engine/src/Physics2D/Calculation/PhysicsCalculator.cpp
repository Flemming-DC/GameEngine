#include "PhysicsCalculator.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"
#include "Collider.h"
//#include "glm/ext.hpp" 
#include "glm/gtx/string_cast.hpp"


void PhysicsCalculator::Update()
{
	// potentialCollisions are stored as collider pairs, rather than collision objects
	auto potentialOverlaps = BroadPhase::GetPotentialOverlaps();
	auto overlaps = NarrowPhase::GetOverlaps(potentialOverlaps);

	for (const auto& pair : overlaps)
	{
		auto pos1 = pair.first->GetTransform()->GetPosition();
		auto str1 = pair.first->to_string();
		auto pos2 = pair.second->GetTransform()->GetPosition();
		auto str2 = pair.second->to_string();
		Log(str1 + " at " + glm::to_string(pos1) + " overlaps with " + str2 + " at " + glm::to_string(pos2));
	}


	//set collision data on collider and call collision events
	//solve collisions

}





