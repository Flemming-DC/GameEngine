#pragma once
#include "BareCollider.h"
#include "glm/glm.hpp"


class BareCircleCollider : public BareCollider
{
public:
	static BareCircleCollider MakePoint(vec2 pos);
	static BareCircleCollider Make(vec2 center, float radius_);
	void Setup(ITransform iTransform_, float radius_);
	float GetRadius() const;
	float GetLocalRadius() const { return localRadius; };
	pair<float, float> ShadowAlongNormal(vec2 normal) const override;

private:
	float localRadius; // we prohibit uneven scaling for cirles 
};

