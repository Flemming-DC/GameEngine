#pragma once
#include "Component.h"
#include "Renderable.h"
#include "Material.h"


class RocketEngine : public Component
{
private:
	Material* material = nullptr; // if using ref, then the code somehow acts like a copying took place

	void OnStart() override;
	void OnUpdate() override;

};

