#include "BareCircleCollider.h"
#include "glm/glm.hpp"
#include <algorithm> 



BareCircleCollider BareCircleCollider::MakePoint(vec2 pos)
{
	float epsilon = glm::pow(10.0f, -4.0f);
	return Make(pos, epsilon);
}

BareCircleCollider BareCircleCollider::Make(vec2 center, float radius_)
{
	if (radius_ < glm::pow(10.0f, -8.0f))
		RaiseError("Unrealistically small radius: ", radius_);
	BareCircleCollider col;
	col.Setup(col.MakeTransform(center), radius_); // , quat(), vec2(1.0f)
	return col;
}

void BareCircleCollider::Setup(ITransform iTransform_, float radius_)
{
	iTransform = iTransform_,
	localRadius = radius_;
}

float BareCircleCollider::GetRadius() const
{
	glm::vec3 scale = iTransform.GetScale();

#ifdef _DEBUG
	// replace error checking with constrained proportions
	float error = (scale.x - scale.y) / std::max(0.005f, scale.x + scale.y);
	if (error * error > 0.01f)
		Warning("non-uniform scales are not supported for exact circle colliders");
#endif // _DEBUG

	return scale.x * localRadius;
}

std::pair<float, float> BareCircleCollider::ShadowAlongNormal(glm::vec2 normal) const
{
	float positionAlongNormal = glm::dot((glm::vec2)iTransform.GetPosition(), normal);
	return { positionAlongNormal - GetRadius(), positionAlongNormal + GetRadius() };
}

