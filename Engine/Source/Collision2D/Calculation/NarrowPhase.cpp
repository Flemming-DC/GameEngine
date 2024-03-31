#include "NarrowPhase.h"
#include "RectangleCollider.h"
#include "GlmTools.h"


using namespace std;
using Overlaps = std::vector<std::pair<Collider*, Collider*>>;

Overlaps NarrowPhase::GetOverlaps(Overlaps potentialOverlaps)
{
	Overlaps overlaps;

	for (const auto& colliderPair : potentialOverlaps)
	{
		if (NarrowPhase::IsOverLapping(colliderPair.first, colliderPair.second))
			overlaps.push_back(colliderPair);
	}

	return overlaps;
}


bool NarrowPhase::IsOverLapping(Collider* collider1, Collider* collider2)
{
	if (collider1 == nullptr)
		RaiseError("collider1 is nullptr");
	if (collider2 == nullptr)
		RaiseError("collider2 is nullptr");

	CircleCollider* cirle1 = dynamic_cast<CircleCollider*>(collider1);
	CircleCollider* cirle2 = dynamic_cast<CircleCollider*>(collider2);
	if (cirle1 != nullptr && cirle2 != nullptr)
		return IsOverLapping_CC(*cirle1, *cirle2);

	PolygonCollider* polygon2 = dynamic_cast<PolygonCollider*>(collider2);
	if (cirle1 != nullptr && polygon2 != nullptr)
		return IsOverLapping_CP(*cirle1, *polygon2);

	PolygonCollider* polygon1 = dynamic_cast<PolygonCollider*>(collider1);
	if (polygon1 != nullptr && cirle2 != nullptr)
		return IsOverLapping_CP(*cirle2, *polygon1);

	if (polygon1 != nullptr && polygon2 != nullptr)
		return IsOverLapping_PP(*polygon1, *polygon2);

	RaiseError(
		"Failed to find a function for computing overlap between colliders\n"
		"collider1: " + collider1->to_string() +
		"collider2: " + collider2->to_string()
		);
	return false; // this is merely to make the warnings shut up
}

bool NarrowPhase::IsOverLapping_CC(CircleCollider& circle1, CircleCollider& circle2)
{
	glm::vec3 displacement = circle1.GetTransform().GetPosition() - circle2.GetTransform().GetPosition();
	float sqrDistance = displacement.x * displacement.x + displacement.y * displacement.y;
	float sqrSumRadii = (circle1.GetRadius() + circle2.GetRadius()) * (circle1.GetRadius() + circle2.GetRadius());
	return sqrDistance <= sqrSumRadii;
}

bool NarrowPhase::IsOverLapping_CP(CircleCollider& circle, PolygonCollider& polygon)
{
	// we use a modified version of the SAT algorithm, in which the we treat 
	// the direction from the circle to closest point in the polygon as the only relevant
	// normal on the circle https://www.youtube.com/watch?v=vWs33LVrs74&ab_channel=Two-BitCoding

	// we use world coordinates
	glm::vec2 circlePosition = circle.GetTransform().GetPosition();

	int count = (int)polygon.GetLocalPosition2Ds().size();
	for (int i = 0; i < count; i++)
	{
		std::pair<float, float> circleShadow = circle.ShadowAlongNormal(polygon.GetNormalByIndex(i));
		auto polygonShadow = polygon.ShadowAlongNormal(polygon.GetNormalByIndex(i));
		bool noOverlap = circleShadow.first > polygonShadow.second || polygonShadow.first > circleShadow.second;
		if (noOverlap)
			return false;
	}

	// now we consider the shadow along the circle's effective normal.
	float minSqrDistance = INFINITY;
	glm::vec2 closestCorner; // evt. put GetClosestCorner calculation inside the collider class
	for (glm::vec2 localPosition2D : polygon.GetLocalPosition2Ds())
	{
		auto position2D = polygon.GetTransform().ToWorldSpace(localPosition2D, true);
		float sqrDistance = glm::dot(position2D - circlePosition, position2D - circlePosition);
		if (sqrDistance < minSqrDistance)
		{
			minSqrDistance = sqrDistance;
			closestCorner = position2D;
		}
	}
	glm::vec2 circleNormal = glm::normalize(closestCorner - circlePosition);
	if (glm::HasNAN(circleNormal) && minSqrDistance < 0.001f)
	{
		// this blockblock activates if closestCorner = circlePosition, in which case we will replace closestCorner with polyPosition
		glm::vec2 polyPosition = polygon.GetTransform().GetPosition();
		circleNormal = glm::normalize(polyPosition - circlePosition); 
	}
	if (glm::HasNAN(circleNormal))
		RaiseError("NAN encountered");

	std::pair<float, float> circleShadow = circle.ShadowAlongNormal(circleNormal);
	auto polygonShadow = polygon.ShadowAlongNormal(circleNormal);
	bool noOverlap = circleShadow.first > polygonShadow.second || polygonShadow.first > circleShadow.second;
	if (noOverlap)
		return false;

	return true;
}

bool NarrowPhase::IsOverLapping_PP(PolygonCollider& polygon1, PolygonCollider& polygon2)
{
	// we use the SAT algorithm https://www.youtube.com/watch?v=59BTXB-kFNs&ab_channel=Nybbit
	
	int count1 = (int)polygon1.GetLocalPosition2Ds().size();
	int count2 = (int)polygon2.GetLocalPosition2Ds().size();

	// we determine which polygon is simplest (in terms of position count)
	PolygonCollider& simplePolygon = count1 < count2 ? polygon1 : polygon2;
	PolygonCollider& complexPolygon = count1 >= count2 ? polygon1 : polygon2;
	int simpleCount = count1 < count2 ? count1 : count2;
	int complexCount = count1 >= count2 ? count1 : count2;

	// we loop over the complex polygon first, so that CalculateShadowAlongNormal acts on
	// the simple polygon, which maximize the chance of an early exit
	for (int i = 0; i < complexCount; i++)
	{
		auto complexShadow = complexPolygon.ShadowAlongNormal(complexPolygon.GetNormalByIndex(i));
		auto simpleShadow = simplePolygon.ShadowAlongNormal(complexPolygon.GetNormalByIndex(i));
		bool noOverlap = complexShadow.first > simpleShadow.second || simpleShadow.first > complexShadow.second;
		if (noOverlap)
			return false;
	}

	for (int i = 0; i < simpleCount; i++)
	{
		auto complexShadow = complexPolygon.ShadowAlongNormal(simplePolygon.GetNormalByIndex(i));
		auto simpleShadow = simplePolygon.ShadowAlongNormal(simplePolygon.GetNormalByIndex(i));
		bool noOverlap = complexShadow.first > simpleShadow.second || simpleShadow.first > complexShadow.second;
		if (noOverlap)
			return false;
	}


	return true;
}





