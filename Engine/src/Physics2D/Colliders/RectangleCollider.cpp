#include "RectangleCollider.h"
#include "ErrorChecker.h"
#include "glm/glm.hpp"



void RectangleCollider::SetSize(glm::vec2 size_)
{
	size = size_; // bruges til save / load;
	glm::vec2 halfSize = 0.5f * size_;
	std::vector<glm::vec2> position2Ds = 
	{
		{-halfSize.x, -halfSize.y }, // LD
		{-halfSize.x,  halfSize.y }, // LU
		{ halfSize.x,  halfSize.y }, // RU
		{ halfSize.x, -halfSize.y }, // RD
	};
	Setup(position2Ds);
}



void RectangleCollider::Save(YAML::Node& node) const
{
	node["size"] = size;
}

void RectangleCollider::Load(const YAML::Node& node)
{
	SetSize(node["size"].as<glm::vec2>());
}



