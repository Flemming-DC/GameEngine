#include "NarrowPhase.h"
#include "RectangleCollider.h"
#include "GlmTools.h"

Shorts;
using Overlaps = vector<pair<Collider*, Collider*>>;
typedef const BareCircleCollider& circle;
typedef const BarePolygonCollider& poly;


Overlaps NarrowPhase::GetOverlaps(Overlaps potentialOverlaps)
{
	Overlaps overlaps;

	for (const auto& colliderPair : potentialOverlaps)
	{
		Assert(colliderPair.first, "collider1 is nullr");
		Assert(colliderPair.second, "collider2 is null");
		if (NarrowPhase::IsOverLapping(colliderPair.first->Bare(), colliderPair.second->Bare()))
			overlaps.push_back(colliderPair);
	}

	return overlaps;
}


bool NarrowPhase::IsOverLapping(const BareCollider& collider1, const BareCollider& collider2)
{
	bool isCircle1 = (typeid(collider1) == typeid(circle));
	bool isPoly1 = (typeid(collider1) == typeid(poly));
	bool isCircle2 = (typeid(collider2) == typeid(circle));
	bool isPoly2 = (typeid(collider2) == typeid(poly));

	if (isCircle1 && isCircle2)
		return IsOverLapping_CC(static_cast<circle>(collider1), static_cast<circle>(collider2));
	else if (isCircle1 && isPoly2)
		return IsOverLapping_CP(static_cast<circle>(collider1), static_cast<poly>(collider2));
	else if (isPoly1 && isCircle2)
		return IsOverLapping_CP(static_cast<circle>(collider2), static_cast<poly>(collider1));
	else if (isPoly1 && isPoly2)
		return IsOverLapping_PP(static_cast<poly>(collider1), static_cast<poly>(collider2));

	RaiseError(
		"Failed to find a function for computing overlap between colliders\n",
		"Type of collider1: ", Tools::TypeName(collider1),
		"Type of collider2: ", Tools::TypeName(collider2));
	return false; // this return-statement serves merely to make the warnings shut up
}

bool NarrowPhase::IsOverLapping_CC(circle circle1, circle circle2)
{
	vec3 displacement = circle1.iTransform.GetPosition() - circle2.iTransform.GetPosition();
	float sqrDistance = displacement.x * displacement.x + displacement.y * displacement.y;
	float sqrSumRadii = (circle1.GetRadius() + circle2.GetRadius()) * (circle1.GetRadius() + circle2.GetRadius());
	return sqrDistance <= sqrSumRadii;
}

bool NarrowPhase::IsOverLapping_CP(circle circle, poly polygon)
{
	// we use a modified version of the SAT algorithm, in which the we treat 
	// the direction from the circle to closest point in the polygon as the only relevant
	// normal on the circle https://www.youtube.com/watch?v=vWs33LVrs74&ab_channel=Two-BitCoding

	// we use world coordinates
	vec2 circlePosition = circle.iTransform.GetPosition();

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
	vec2 closestCorner; // evt. put GetClosestCorner calculation inside the collider class
	for (vec2 localPosition2D : polygon.GetLocalPosition2Ds())
	{
		auto position2D = polygon.iTransform.ToWorldSpace(localPosition2D, true);
		float sqrDistance = glm::dot(position2D - circlePosition, position2D - circlePosition);
		if (sqrDistance < minSqrDistance)
		{
			minSqrDistance = sqrDistance;
			closestCorner = position2D;
		}
	}
	vec2 circleNormal = glm::normalize(closestCorner - circlePosition);
	if (glm::HasNAN(circleNormal) && minSqrDistance < 0.00001f)
	{
		// this blockblock activates if closestCorner = circlePosition, in which case we will replace closestCorner with polyPosition
		vec2 polyPosition = polygon.iTransform.GetPosition();
		circleNormal = glm::normalize(polyPosition - circlePosition); 
		if (glm::HasNAN(circleNormal))
			return true; // since minSqrDistance is tiny, then it can't be very wrong to return true.
	}
	else 
	{
		Deny(glm::HasNAN(circleNormal),
			"NAN encountered"); // if NAN occurs for reasons other than tiny minSqrDistance, then I dont know what to do.
	} 
	pair<float, float> circleShadow = circle.ShadowAlongNormal(circleNormal);
	auto polygonShadow = polygon.ShadowAlongNormal(circleNormal);
	bool noOverlap = circleShadow.first > polygonShadow.second || polygonShadow.first > circleShadow.second;
	if (noOverlap)
		return false;

	return true;
}

bool NarrowPhase::IsOverLapping_PP(poly polygon1, poly polygon2)
{
	ProfileFunc;
	// we use the SAT algorithm https://www.youtube.com/watch?v=59BTXB-kFNs&ab_channel=Nybbit
	
	ProfileLine(int count1 = (int)polygon1.GetLocalPosition2Ds().size(););
	ProfileLine(int count2 = (int)polygon2.GetLocalPosition2Ds().size(););

	// we determine which polygon is simplest (in terms of position count)
	ProfileLine(poly simplePolygon = count1 < count2 ? polygon1 : polygon2;);
	ProfileLine(poly complexPolygon = count1 >= count2 ? polygon1 : polygon2;);
	ProfileLine(int simpleCount = count1 < count2 ? count1 : count2;);
	ProfileLine(int complexCount = count1 >= count2 ? count1 : count2;);

	// we loop over the complex polygon first, so that CalculateShadowAlongNormal acts on
	// the simple polygon, which maximize the chance of an early exit
	for (int i = 0; i < complexCount; i++)
	{
		ProfileLine(auto complexShadow = complexPolygon.ShadowAlongNormal(complexPolygon.GetNormalByIndex(i)););
		ProfileLine(auto simpleShadow = simplePolygon.ShadowAlongNormal(complexPolygon.GetNormalByIndex(i)););
		ProfileLine(bool noOverlap = complexShadow.first > simpleShadow.second || simpleShadow.first > complexShadow.second;);
		if (noOverlap)
			return false;
	}

	for (int i = 0; i < simpleCount; i++)
	{
		ProfileLine(auto complexShadow = complexPolygon.ShadowAlongNormal(simplePolygon.GetNormalByIndex(i)););
		ProfileLine(auto simpleShadow = simplePolygon.ShadowAlongNormal(simplePolygon.GetNormalByIndex(i)););
		ProfileLine(bool noOverlap = complexShadow.first > simpleShadow.second || simpleShadow.first > complexShadow.second;);
		if (noOverlap)
			return false;
	}

	return true;
}





