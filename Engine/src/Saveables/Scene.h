#pragma once
#include <string>
#include <vector>
#include "Entity.h"


class Scene
{
public:
	//Scene(std::string name);
	virtual std::vector<Entity>& MakeEntities() = 0;

protected:
	std::string name;
	std::vector<Entity> entities;

	void LoadSpatialData(const std::vector<Entity>& entities);
};

