#pragma once
#include "Component.h"


class RocketEngine : public Component
{
private:
	void OnStart() override;
	void OnUpdate() override;
	//void OnDestroy() override;
};

