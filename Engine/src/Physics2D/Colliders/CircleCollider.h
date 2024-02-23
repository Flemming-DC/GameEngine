#pragma once
#include "Collider.h"
#include "glm/glm.hpp"


// we pro
class CircleCollider : public Collider
{
public:
	void SetLocalRadius(float radius_);
	float GetRadius() const;// { return radius; };
	std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const override;
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;

private:
	float localRadius; // this is just localRadius, but we prohibit scaling for cirles 
};

