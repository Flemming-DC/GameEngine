#pragma once
#include "Engine.h"
#include "Material.h"

class GameLogic : public Component
{
public:
	void SetByInspector(const Material& material); // temporary function

private:
	float increment = 0.01f;
	glm::vec4 color = glm::vec4(1.0f);
	Material material; // show in inspector

	void OnUpdate() override;


};

