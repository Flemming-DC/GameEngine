#pragma once
#include "Component.h"
#include "Transform.h"
#include "Collider.h"
#include "RocketEngine.h"

class Player : public Component
{
public:
	static Transform& transform();
	static Collider& collider();
	static RocketEngine& rocketEngine();

private:
	void OnStart() override;
	void OnDestroy() override;
	virtual int InitOrder() { return -10; }
};

