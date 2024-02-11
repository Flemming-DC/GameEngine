#include "NarrowPhase.h"
#include "RectangleCollider.h"
#include "GlmTools.h"

// rectangle on rectangle
// position ok, but rotation is ignored and scale seems to be inverted

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
	CircleCollider* cirle1 = dynamic_cast<CircleCollider*>(collider1);
	CircleCollider* cirle2 = dynamic_cast<CircleCollider*>(collider2);
	if (cirle1 != nullptr && cirle2 != nullptr)
		return IsOverLapping(cirle1, cirle2);

	PolygonCollider* polygon2 = dynamic_cast<PolygonCollider*>(collider2);
	if (cirle1 != nullptr && polygon2 != nullptr)
		return IsOverLapping(cirle1, polygon2);

	PolygonCollider* polygon1 = dynamic_cast<PolygonCollider*>(collider1);
	if (polygon1 != nullptr && cirle2 != nullptr)
		return IsOverLapping(polygon1, cirle2);

	if (polygon1 != nullptr && polygon2 != nullptr)
		return IsOverLapping(polygon1, polygon2);

	RaiseError(
		"Failed to find a function for computing overlap between colliders\n"
		"collider1: " + collider1->to_string() +
		"collider2: " + collider2->to_string()
		);
	return false; // this is merely to make the warnings shut up
}

bool NarrowPhase::IsOverLapping(CircleCollider* circle1, CircleCollider* circle2)
{
	glm::vec3 displacement = circle1->GetTransform()->GetPosition() - circle2->GetTransform()->GetPosition();
	float sqrDistance = displacement.x * displacement.x + displacement.y * displacement.y;
	float sqrSumRadii = (circle1->GetRadius() + circle2->GetRadius()) * (circle1->GetRadius() + circle2->GetRadius());
	return sqrDistance <= sqrSumRadii;
}

bool NarrowPhase::IsOverLapping(CircleCollider* circle, PolygonCollider* polygon)
{
	// we use a modified version of the SAT algorithm, in which the we treat 
	// the direction from the circle to closest point in polygon as the only relevant
	// normal on the circle https://www.youtube.com/watch?v=vWs33LVrs74&ab_channel=Two-BitCoding

	// we use world coordinates
	glm::vec2 circlePosition = circle->GetTransform()->GetPosition();

	int count = (int)polygon->GetLocalPosition2Ds().size();
	for (int i = 0; i < count; i++)
	{
		float circlePositionAlongNormal = glm::dot(circlePosition, polygon->GetNormalByIndex(i));
		std::pair<float, float> circleShadow = {
			circlePositionAlongNormal - circle->GetRadius(), circlePositionAlongNormal + circle->GetRadius() };
		auto polygonShadow = polygon->CalculateShadowAlongNormal(polygon->GetNormalByIndex(i));
		bool noOverlap = circleShadow.first > polygonShadow.second || polygonShadow.first > circleShadow.second;
		if (noOverlap)
			return false;
	}

	// now we consider the shadow along the circle's effective normal.
	float minSqrDistance = INFINITY;
	glm::vec2 closestPoint; // evt. put GetClosestPoint calculation inside the collider class
	for (glm::vec2 localPosition2D : polygon->GetLocalPosition2Ds())
	{
		auto position2D = polygon->GetTransform()->ToWorldSpace(localPosition2D, true);
		float sqrDistance = glm::dot(position2D - circlePosition, position2D - circlePosition);
		if (sqrDistance < minSqrDistance)
		{
			minSqrDistance = sqrDistance;
			closestPoint = position2D;
		}
	}
	glm::vec2 circleNormal = glm::normalize(closestPoint - circlePosition);
	if (Tools::HasNAN(circleNormal))
		RaiseError("NAN encountered");

	float circlePositionAlongNormal = glm::dot(circlePosition, circleNormal);
	std::pair<float, float> circleShadow = {
		circlePositionAlongNormal - circle->GetRadius(), circlePositionAlongNormal + circle->GetRadius() };
	auto polygonShadow = polygon->CalculateShadowAlongNormal(circleNormal, true);
	bool noOverlap = circleShadow.first > polygonShadow.second || polygonShadow.first > circleShadow.second;
	if (noOverlap)
		return false;

	return true;
}

bool NarrowPhase::IsOverLapping(PolygonCollider* polygon1, PolygonCollider* polygon2)
{
	// we use the SAT algorithm https://www.youtube.com/watch?v=59BTXB-kFNs&ab_channel=Nybbit
	
	int count1 = (int)polygon1->GetLocalPosition2Ds().size();
	int count2 = (int)polygon2->GetLocalPosition2Ds().size();

	// we determine which polygon is simplest (in terms of position count)
	PolygonCollider* simplePolygon = count1 < count2 ? polygon1 : polygon2;
	PolygonCollider* complexPolygon = count1 >= count2 ? polygon1 : polygon2;
	int simpleCount = count1 < count2 ? count1 : count2;
	int complexCount = count1 >= count2 ? count1 : count2;

	// we loop over the complex polygon first, so that CalculateShadowAlongNormal acts on
	// the simple polygon, which maximize the chance of an early exit
	for (int i = 0; i < complexCount; i++)
	{
		auto complexShadow = complexPolygon->CalculateShadowAlongNormal(complexPolygon->GetNormalByIndex(i));
		auto simpleShadow = simplePolygon->CalculateShadowAlongNormal(complexPolygon->GetNormalByIndex(i));
		bool noOverlap = complexShadow.first > simpleShadow.second || simpleShadow.first > complexShadow.second;
		if (noOverlap)
			return false;
	}

	for (int i = 0; i < simpleCount; i++)
	{
		auto complexShadow = complexPolygon->CalculateShadowAlongNormal(simplePolygon->GetNormalByIndex(i));
		auto simpleShadow = simplePolygon->CalculateShadowAlongNormal(simplePolygon->GetNormalByIndex(i));
		bool noOverlap = complexShadow.first > simpleShadow.second || simpleShadow.first > complexShadow.second;
		if (noOverlap)
			return false;
	}


	return true;
}









