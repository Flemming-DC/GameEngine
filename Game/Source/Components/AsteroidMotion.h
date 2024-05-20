#pragma once
#include "Component.h"
#include "GlmTools.h"


class AsteroidMotion : public Component
{
private:
	glm::vec2 velocity;

	void OnUpdate() override;
};

