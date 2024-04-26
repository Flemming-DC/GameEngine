#include "PolygonCollider.h"
#include "ErrorChecker.h"
#include "Gizmo.h"


void PolygonCollider::Setup(std::vector<glm::vec2> localPosition2Ds_)
{
	bare.Setup(MakeTransformInterface(), localPosition2Ds_);
	/*
	if (UuidCreator::IsInitialized(gizmoID))
		Gizmo::Remove(gizmoID);
	gizmoID = Gizmo::Make(localPosition2Ds_, &GetTransform());
	*/
}




void PolygonCollider::Save(YAML::Node& node) const
{
	node["localPosition2Ds"] = bare.GetLocalPosition2Ds();
}

void PolygonCollider::Load(const YAML::Node& node)
{
	Setup(node["localPosition2Ds"].as<std::vector<glm::vec2>>());
}




