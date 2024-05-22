#include "CircleCollider.h"
#include "glm/glm.hpp"
#include "GlmCheck.h"
#include <algorithm> 


void CircleCollider::OnStart()
{
	Collider::OnStart();
	GetTransform().requireUniformScale = true;
	if (bare.GetLocalRadius() < GlmCheck::realisticallySmall)
		Setup(0.5f, glm::vec2(0.0f));
}


void CircleCollider::Setup(float radius_, glm::vec2 center_)
{
	center = center_;
	bare.Setup(MakeTransformInterface(center), radius_);

	//glm::vec2 center = glm::vec2(0.0f); // evt. make this a variable
}



void CircleCollider::Save(YAML::Node& node) const
{
	node["localRadius"] = bare.GetLocalRadius();
	node["center"] = center;
}

void CircleCollider::Load(const YAML::Node& node)
{
	auto radius_ = node["localRadius"].as<float>();
	auto center_ = node["center"].as<glm::vec2>();
	Setup(radius_, center_);
}


