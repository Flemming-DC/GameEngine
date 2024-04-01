#include "CircleCollider.h"
#include "glm/glm.hpp"
#include <algorithm> 


void CircleCollider::SetLocalRadius(float radius_)
{
	localRadius = radius_;
	glm::vec2 center = glm::vec2(0.0f); // evt. make this a variable
	gizmoID = Gizmo::MakeCircle(center, radius_, GetTransform());
}


float CircleCollider::GetRadius() const
{
	auto scale = GetTransform().GetScale();

	// replace error checking with constrained proportions
	float error = (scale.x - scale.y) / std::max(0.005f, scale.x + scale.y);
	if (error * error > 0.01f)
		Warning("non-uniform scales are not supported for exact circle colliders");

	return scale.x * localRadius;
}

std::pair<float, float> CircleCollider::ShadowAlongNormal(glm::vec2 normal) const
{
	float positionAlongNormal = glm::dot((glm::vec2)GetTransform().GetPosition(), normal);
	return { positionAlongNormal - GetRadius(), positionAlongNormal + GetRadius() };
}


void CircleCollider::Save(YAML::Node& node) const
{
	node["localRadius"] = localRadius;
}

void CircleCollider::Load(const YAML::Node& node)
{
	SetLocalRadius(node["localRadius"].as<float>());
}


