#include "CollisionChecker.h"

Collider* CollisionChecker::RayCast(
	glm::vec2 startPosition, glm::vec2 direction, float distance)
{
	// some kind of broad phase filter ???

	float minPosAlongDir = INFINITY;
	Collider* nearestHit = nullptr;

	float startPosAlongDir = glm::dot(startPosition, direction);
	std::pair<float, float> rayShadow = { startPosAlongDir, startPosAlongDir + distance };
	auto normal = glm::vec2(-direction.y, direction.x);
	float startPosAlongNormal = glm::dot(startPosition, normal);

	for (const auto colID : Collider::GetAllColliders())
	{
		auto& col = Entity::GetComponent<Collider>(colID);
		// checking for overlap
		auto shadowNormal = col.ShadowAlongNormal(normal);
		bool noOverlapNormal = shadowNormal.first > startPosAlongNormal || startPosAlongNormal > shadowNormal.second;
		if (noOverlapNormal)
			continue;
		auto shadow = col.ShadowAlongNormal(direction);
		bool noOverlap = shadow.first > rayShadow.second || rayShadow.first > shadow.second;
		if (noOverlap)
			continue;

		// updating minPosAlongDir, nearestHit
		float posAlongDir = shadow.first;
		if (posAlongDir < minPosAlongDir)
		{
			minPosAlongDir = posAlongDir;
			nearestHit = &col;
		}
	}
	return nearestHit;

}

std::vector<Collider*> CollisionChecker::RayOverlaps(
	glm::vec2 startPosition, glm::vec2 direction, float distance)
{
	// some kind of broad phase filter ???

	std::vector<Collider*> hits = {};

	float startPosAlongDirection = glm::dot(startPosition, direction);
	std::pair<float, float> rayShadow = { startPosAlongDirection, startPosAlongDirection + distance };
	auto normal = glm::vec2(-direction.y, direction.x);
	float startPosAlongNormal = glm::dot(startPosition, normal);

	for (const auto& colID : Collider::GetAllColliders())
	{
		auto& col = Entity::GetComponent<Collider>(colID);
		// checking for overlap
		auto shadowNormal = col.ShadowAlongNormal(normal);
		bool noOverlapNormal = shadowNormal.first > startPosAlongNormal || startPosAlongNormal > shadowNormal.second;
		if (noOverlapNormal)
			continue;
		auto shadow = col.ShadowAlongNormal(direction);
		bool noOverlap = shadow.first > rayShadow.second || rayShadow.first > shadow.second;
		if (noOverlap)
			continue;
		// appending to list
		hits.push_back(&col);
	}
	return hits;

}

