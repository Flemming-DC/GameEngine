#include "PolygonCollider.h"
#include "ErrorChecker.h"
#include "logger.h"

void PolygonCollider::OnStart()
{
	Collider::OnStart();
	if (!bare.GetLocalPosition2Ds().empty())
		return;
	glm::vec2 halfSize = glm::vec2(0.5f);
	std::vector<glm::vec2> position2Ds =
	{
		{-halfSize.x, -halfSize.y }, // LD
		{-halfSize.x,  halfSize.y }, // LU
		{ halfSize.x,  halfSize.y }, // RU
		{ halfSize.x, -halfSize.y }, // RD
	};
	SetupPoly(position2Ds);
}

void PolygonCollider::SetupPoly(std::vector<glm::vec2> localPosition2Ds_)
{
	bare.Setup(MakeTransformInterface(), localPosition2Ds_, GetTransform().IsStatic());
}




void PolygonCollider::Save(YAML::Node& node) const
{
	node["localPosition2Ds"] = bare.GetLocalPosition2Ds();
}

void PolygonCollider::Load(const YAML::Node& node)
{
	SetupPoly(node["localPosition2Ds"].as<std::vector<glm::vec2>>());
}




