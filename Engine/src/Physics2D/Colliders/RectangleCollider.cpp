#include "RectangleCollider.h"
#include "ErrorChecker.h"
#include "glm/glm.hpp"



void RectangleCollider::SetSize(glm::vec2 size)
{
	glm::vec2 halfSize = 0.5f * size;
	std::vector<glm::vec2> position2Ds = 
	{
		{-halfSize.x, -halfSize.y }, // LD
		{-halfSize.x,  halfSize.y }, // LU
		{ halfSize.x,  halfSize.y }, // RU
		{ halfSize.x, -halfSize.y }, // RD
	};
	Setup(position2Ds);
}





