#pragma once
#include "Engine.h"
#include <stduuid/uuid.h>


class DemoScene : public Scene
{
public:
	
	void PurelyManualSetup() override;
	void ManualSetup() override;

private:
	//void DemoScene::HelloEnter(Collider* other);
	//void DemoScene::HelloExit(Collider* other);
	//static void DemoScene::HelloEnter(Collider* other);
	//static void DemoScene::HelloExit(Collider* other);

};

