#include "ColQuery.h"
#include "NarrowPhase.h"
Shorts

Collider* ColQuery::RayCast(
	vec2 startPosition, vec2 direction, float distance)
{
	// some kind of broad phase filter ???

	float minPosAlongDir = INFINITY;
	Collider* nearestHit = nullptr;

	float startPosAlongDir = glm::dot(startPosition, direction);
	pair<float, float> rayShadow = { startPosAlongDir, startPosAlongDir + distance };
	auto normal = vec2(-direction.y, direction.x);
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

vector<Collider*> ColQuery::RayOverlaps(
	vec2 startPosition, vec2 direction, float distance)
{
	// some kind of broad phase filter ???

	vector<Collider*> hits = {};

	float startPosAlongDirection = glm::dot(startPosition, direction);
	pair<float, float> rayShadow = { startPosAlongDirection, startPosAlongDirection + distance };
	auto normal = vec2(-direction.y, direction.x);
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

Collider* ColQuery::TryGetOverlap(const BareCollider& collider)
{
	// introduce a broadphase first
	vector<Collider*> overlappingColliders;
	for (const auto& colliderID : Collider::GetAllColliders())
	{
		auto& colComp = Entity::GetComponent<Collider>(colliderID);
		if (NarrowPhase::IsOverLapping(collider, colComp.Bare()))
			return &colComp;
	}
	return nullptr;
}

vector<Collider*> ColQuery::GetOverlaps(const BareCollider& collider)
{
	// introduce a broadphase first
	vector<Collider*> overlappingColliders;
	for (const auto& colliderID : Collider::GetAllColliders())
	{
		auto& colComp = Entity::GetComponent<Collider>(colliderID);
		if (NarrowPhase::IsOverLapping(collider, colComp.Bare()))
			overlappingColliders.push_back(&colComp);
	}
	return overlappingColliders;
}

bool ColQuery::IsOverlapping(const BareCollider& col1, const BareCollider& col2)
{
	// evt. apply a broadphase check first
	return NarrowPhase::IsOverLapping(col1, col2);
}