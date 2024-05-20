#pragma once
#include "Component.h"
#include "Collider.h"

class Impact : public Component
{
private:
	void OnStart() override;

	void Kill(Collider& other);
};

