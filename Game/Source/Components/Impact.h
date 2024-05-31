#pragma once
#include "Component.h"
#include "Collider.h"

class Impact : public Component
{
public:
	inline bool IsRock() { return isRock; };

private:
	unsigned int funcID;
	bool isRock;

	void OnStart() override;
	void OnDestroy() override;

	void Kill(Collider& other);
};

