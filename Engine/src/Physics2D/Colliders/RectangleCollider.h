#pragma once
#include "Collider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "glm/glm.hpp"

// hyp: evt. treat rectangles as the subcatagory of polygons the really are.
class RectangleCollider : public PolygonCollider
{
public:

	void SetSize(glm::vec2 size);

private:
	glm::vec2 size;


};

