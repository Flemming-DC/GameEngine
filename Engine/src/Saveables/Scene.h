#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include "Register.h"


class Scene
{
public:
	void Setup();
	virtual std::vector<uuids::uuid> PurelyManualSetup() = 0;
	virtual void ManualSetup() = 0;
	void Load(); // load from file
	void Save(); // save to file
	//inline uuids::uuid GetID() const { return id; }

protected:
	std::vector<uuids::uuid> entityIDs; // is this dublicate info? They are also in the register. 
	// what about map: name -> id for convenience
private:
	uuids::uuid id;
	//static Register<Scene> register_;
	std::string Path() { return "res/Scenes/" + Tools::type_as_string(*this) + ".yml"; } // evt. store path rather than name
};

