#pragma once
#include "Collider.h"
#include "BarePolygonCollider.h"
#include "glm/glm.hpp"


// the polygon has to be convex. evt. introduce a check for this
class PolygonCollider : public Collider
{
public:
	BarePolygonCollider bare;

	void Setup(std::vector<glm::vec2> localPosition2Ds);
	// add position and remove position would be useful in an editor
	void SetPosition(int index, glm::vec2 newPosition) { bare.SetPosition(index, newPosition); }
	void AddPositionAfter(int priorPositionIndex) { bare.AddPositionAfter(priorPositionIndex); }
	void RemovePosition(int index) { bare.RemovePosition(index); }

	//inline std::pair<float, float> ShadowAlongNormal(glm::vec2 normal) const override { return bare.ShadowAlongNormal(normal); };
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	const BareCollider& Bare() const override { return bare; };
	int InitOrder() override { return -1000; }

private:
};

