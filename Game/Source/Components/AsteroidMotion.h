#pragma once
#include "Component.h"
#include "GlmTools.h"


class AsteroidMotion : public Component
{
private:
	glm::vec2 velocity;
	float speed = 1.0f;
	float angularSpeed = 0.5f;

	void OnUpdate() override;
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
};

