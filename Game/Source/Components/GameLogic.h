#pragma once
#include "Engine.h"
#include "Material.h"

class GameLogic : public Component
{
public:
	int InitOrder() override { return 1; }

private:
	float increment = 0.01f;
	glm::vec4 color = glm::vec4(1.0f);
	Material* material = nullptr; // if using ref, then the code somehow acts like a copying took place
	unsigned int onEnterIndex = -1;
	unsigned int onExitIndex = -1;

	void OnStart() override;
	void OnUpdate() override;
	void OnDestroy() override;

	void OldCollisionTests();
	void CollisionTests();
};

