#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include "Register.h"


class Scene
{
public:
	void Setup(std::string name_);
	virtual std::vector<uuids::uuid> MakeEntities() = 0;
	void Load(); // load from file
	void Save(); // save to file
	//inline uuids::uuid GetID() const { return id; }

protected:
	std::vector<uuids::uuid> entityIDs; // is this dublicate info? They are also in the register. 
	// what about map: name -> id for convenience
private:
	std::string name;
	uuids::uuid id;
	//static Register<Scene> register_;

	std::string Path() { return "res/Scenes/" + name + ".yml"; } // evt. store path rather than name
};

