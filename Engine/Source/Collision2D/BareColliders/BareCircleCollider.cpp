#include "BareCircleCollider.h"
#include "glm/glm.hpp"
#include <algorithm> 
#include "GlmCheck.h"



BareCircleCollider BareCircleCollider::MakePoint(vec2 pos)
{
	return Make(pos, Realistic_p(0));
}

BareCircleCollider BareCircleCollider::Make(vec2 center, float radius_)
{
	BareCircleCollider col;
	col.Setup(col.MakeTransform(center), radius_, false); 
	return col;
}

void BareCircleCollider::Setup(ITransform iTransform_, float radius_, bool isStatic)
{
	// isStatic is currently unused, but available for use in optimizations.
	iTransform = iTransform_,
	localRadius = Check_p(radius_);
}

float BareCircleCollider::GetRadius() const
{
	glm::vec3 scale = iTransform.GetScale();

	InDebug(
		// replace error checking with constrained proportions
		float error = (scale.x - scale.y) / std::max(0.005f, scale.x + scale.y);
		if (error * error > 0.01f)
			Warning("non-uniform scales are not supported for circle colliders. Using scale.x for both x and y.");
	);
	return scale.x * localRadius;
}

std::pair<float, float> BareCircleCollider::ShadowAlongNormal(glm::vec2 normal) const
{
	float positionAlongNormal = glm::dot((glm::vec2)iTransform.GetPosition(), normal);
	return { positionAlongNormal - GetRadius(), positionAlongNormal + GetRadius() };
}


std::vector<glm::vec2> BareCircleCollider::Positions() const
{
	vector<vec2> positions;
	auto center = iTransform.GetPosition();
	auto radius = GetRadius();
	int segmentCount = 40;
	float pi = 3.14159265358979323846f;
	for (int i = 0; i < segmentCount; ++i)
	{
		float angle = (float)i / (float)segmentCount * 2.0f * pi;
		positions.push_back({
			center.x + radius * std::cos(angle),
			center.y + radius * std::sin(angle) });
	}
	return positions;
}


BoundingBox BareCircleCollider::GetBoundingBox() const
{
	auto center = iTransform.GetPosition();
	auto radius = GetRadius();
	// minX, minY, maxX, maxY
	return { 
		center.x - radius, 
		center.y - radius, 
		center.x + radius, 
		center.y + radius };
}
