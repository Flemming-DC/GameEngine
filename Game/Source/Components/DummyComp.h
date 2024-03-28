#pragma once
#include "Engine.h"
#include "InputAction.h"

class DummyComp : public Component
{
public:

private:
	uuids::uuid gizmoID;
	unsigned int onConnectedChangedIndex = -1;

	void OnStart() override;
	void OnUpdate() override;
	void OnDestroy() override;

	void OldTests();
};


