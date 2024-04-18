#include "CircleCollider.h"
#include "Gizmo.h"
#include "glm/glm.hpp"
#include <algorithm> 


void CircleCollider::OnStart()
{
	Collider::OnStart();
	GetTransform().requireUniformScale = true;
}


void CircleCollider::Setup(float radius_)
{
	bare.Setup(MakeTransformInterface(), radius_);

	glm::vec2 center = glm::vec2(0.0f); // evt. make this a variable
	gizmoID = Gizmo::MakeCircle(center, radius_, GetTransform());
}



void CircleCollider::Save(YAML::Node& node) const
{
	node["localRadius"] = bare.GetLocalRadius();
}

void CircleCollider::Load(const YAML::Node& node)
{
	Setup(node["localRadius"].as<float>());
}


