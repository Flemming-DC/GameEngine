#pragma once
#include "Collider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "glm/glm.hpp"


class RectangleCollider : public PolygonCollider
{
public:

	void SetSize(glm::vec2 size);

private:
	glm::vec2 size;


};

