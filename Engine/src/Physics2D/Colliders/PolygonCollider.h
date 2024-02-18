#pragma once
#include "Collider.h"
#include "glm/glm.hpp"


// the polygon has to be convex. evt. introduce a check for this
class PolygonCollider : public Collider
{
public:
	void Setup(std::vector<glm::vec2> position2Ds_);
	// add position and remove position would be useful in an editor

	const std::vector<glm::vec2>& GetLocalPosition2Ds() const { return localPosition2Ds; } 
	std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const override;
	const glm::vec2 GetNormalByIndex(int i) const { return GetTransform().ToWorldSpace(localNormals[i], false); };

private:
	std::vector<glm::vec2> localPosition2Ds; // positions in local space i.e. relative to transform.GetPosition()
	std::vector<glm::vec2> localNormals;
	glm::vec2 centerOfMass; // same as average position2D, since we assume uniform density
};

