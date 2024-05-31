#pragma once
#include "Component.h"
#include "Renderable.h"
#include "Material.h"


class RocketEngine : public Component
{
public:
	void Die();

private:
	void OnStart() override;
	void OnUpdate() override;


};

