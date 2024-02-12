#pragma once
#include <string>
#include <vector>
#include "Entity.h"


class Scene
{
public:
	Scene(std::string name);
	virtual std::vector<Entity> MakeEntities() = 0;
	
private:
	std::string name;
	std::vector<Entity> entities;

	void LoadSpatialData(std::vector<Entity> entities);
};

