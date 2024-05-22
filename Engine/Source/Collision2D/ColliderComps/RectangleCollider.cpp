#include "RectangleCollider.h"
#include "ErrorChecker.h"
#include "glm/glm.hpp"



void RectangleCollider::SetupRect(glm::vec2 size_, glm::vec2 center)
{
	size = size_; // bruges til save / load;
	glm::vec2 halfSize = 0.5f * size_;
	std::vector<glm::vec2> position2Ds = 
	{
		center + glm::vec2(-halfSize.x, -halfSize.y), // LD
		center + glm::vec2(-halfSize.x,  halfSize.y), // LU
		center + glm::vec2( halfSize.x,  halfSize.y), // RU
		center + glm::vec2( halfSize.x, -halfSize.y), // RD
	};
	SetupPoly(position2Ds);
}



void RectangleCollider::Save(YAML::Node& node) const
{
	node["size"] = size;
	node["center"] = center;
}

void RectangleCollider::Load(const YAML::Node& node)
{
	auto size_ = node["size"].as<glm::vec2>();
	auto center_ = node["center"].as<glm::vec2>();
	SetupRect(size_, center_);
}



