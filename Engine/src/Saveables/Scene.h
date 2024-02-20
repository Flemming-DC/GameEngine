#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include "Entity.h"


class Scene
{
public:
	void Setup(std::string name_);
	virtual std::vector<uuids::uuid> MakeEntities() = 0;
	void Load(); // load from file
	void Save(); // save to file

protected:
	std::string name;
	std::vector<uuids::uuid> entityIDs;
	// register

	std::string Path(std::string name) { return "res/Scenes/" + name + ".yml"; } // evt. store path rather than name
};

