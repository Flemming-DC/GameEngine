#pragma once
#include "Engine.h"

class DummyComp : public Component
{
public:

private:
	uuids::uuid gizmoID;

	void OnStart() override;
	void OnUpdate() override;


};


