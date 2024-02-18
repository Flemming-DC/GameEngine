#pragma once
#include "Engine.h"


class DemoScene //: public Scene
{
public:
	//DemoScene(const std::string& name);// : Scene(name) {}
	
	std::vector<Entity>& MakeEntities();// override;

private:
	void DemoScene::HelloEnter(Collider* other);
	void DemoScene::HelloExit(Collider* other);
	std::vector<Entity> entities;

};

