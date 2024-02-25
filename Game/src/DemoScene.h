#pragma once
#include "Engine.h"
#include <stduuid/uuid.h>


class DemoScene : public Scene
{
public:
	//DemoScene(const std::string& name) : Scene(name) {}
	
	std::vector<uuids::uuid> PurelyManualSetup() override;
	void ManualSetup() override;
	Entity& GetEntity(int index) { return Entity::register_.Get(entityIDs[index]); } // temp tool for GUI

private:
	//void DemoScene::HelloEnter(Collider* other);
	//void DemoScene::HelloExit(Collider* other);
	static void DemoScene::HelloEnter(Collider* other);
	static void DemoScene::HelloExit(Collider* other);

};

