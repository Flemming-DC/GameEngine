#include "CircleCollider.h"
#include "glm/glm.hpp"
#include "GlmCheck.h"
#include <algorithm> 


void CircleCollider::OnStart()
{
	Collider::OnStart();
	GetTransform().requireUniformScale = true;
	if (bare.GetLocalRadius() < GlmCheck::realisticallySmall)
		Setup(0.5f);
}


void CircleCollider::Setup(float radius_)
{
	bare.Setup(MakeTransformInterface(), radius_);

	//glm::vec2 center = glm::vec2(0.0f); // evt. make this a variable
}



void CircleCollider::Save(YAML::Node& node) const
{
	node["localRadius"] = bare.GetLocalRadius();
}

void CircleCollider::Load(const YAML::Node& node)
{
	Setup(node["localRadius"].as<float>());
}


