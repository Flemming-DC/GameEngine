#pragma once
#include "Collider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "glm/glm.hpp"


class RectangleCollider : public PolygonCollider
{
public:
	inline glm::vec2 Size() const { return size; }
	inline glm::vec2 Center() const { return center; }
	void SetupRect(glm::vec2 size, glm::vec2 center);

	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	int InitOrder() override { return -1000; }

private:
	glm::vec2 center = glm::vec2(0.0f); // relative to transform.position2D
	glm::vec2 size = glm::vec2(1.0f);


};

