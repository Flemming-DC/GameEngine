#pragma once
#include "Component.h"
#include "Collider.h"
//#include "Sound.h"
//#include "GameLiterals.h"

class Impact : public Component
{
public:
	inline bool IsRock() { return isRock; };

private:
	unsigned int funcID;
	bool isRock;
	//Sound* impactSound;

	void OnStart() override;
	void OnDestroy() override;

	void OnColliderEnter(Collider& other);
};

