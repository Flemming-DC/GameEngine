#pragma once
#include "Collider.h"
#include "BareCircleCollider.h"
#include "glm/glm.hpp"



class CircleCollider : public Collider
{
public:
	BareCircleCollider bare;

	void Setup(float radius_);
	//inline std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const override { return bare.ShadowAlongNormal(normal); };
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	const BareCollider& Bare() const override { return bare; };
	//BareCollider& Bare() { return bare; };

private:
};

