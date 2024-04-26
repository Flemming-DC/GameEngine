#pragma once
#include "Engine.h"
#include "InputAction.h"
#include "DemoScene.h"
#include "SecondScene.h"

class DummyComp : public Component
{
public:

private:
	unsigned int onConnectedChangedIndex = -1;
	std::vector<DemoScene> demoScenes;
	std::vector<SecondScene> secondScene;

	void OnStart() override;
	void OnUpdate() override;
	void OnDestroy() override;

	void OldTests();
	void TestInputActions();
};


