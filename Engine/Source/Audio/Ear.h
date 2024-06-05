#pragma once
#include "Component.h"

class Ear : public Component
{
public:
	void SetCone(float innerAngle, float outerAngle, float reductionFactor);
	int InitOrder() override { return -1200; }

private:
	void OnStart() override;
	void OnUpdate() override;

};

