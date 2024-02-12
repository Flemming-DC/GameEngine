#pragma once
#include "Engine.h"


class DemoScene : public Scene
{
public:
	std::vector<Entity> MakeEntities() override;

private:
	void DemoScene::HelloEnter(Collider* other);
	void DemoScene::HelloExit(Collider* other);

};

