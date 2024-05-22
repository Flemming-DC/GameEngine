#pragma once
#include "Collider.h"
#include "BareCircleCollider.h"
#include "glm/glm.hpp"



class CircleCollider : public Collider
{
public:
	BareCircleCollider bare;

	inline glm::vec2 Center() const { return center; }
	//void SetCenter(glm::vec2 size);
	void Setup(float radius_, glm::vec2 center);
	//inline std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const override { return bare.ShadowAlongNormal(normal); };
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	const BareCollider& Bare() const override { return bare; };
	//BareCollider& Bare() { return bare; };
	int InitOrder() override { return -1000; }

private:
	glm::vec2 center = glm::vec2(0.0f); // relative to transform.position2D

	void OnStart() override;

};

