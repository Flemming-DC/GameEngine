#pragma once
#include "Engine.h"
#include <stduuid/uuid.h>


class SecondScene : public Scene
{
public:

	void PurelyManualSetup() override;
	void ManualSetup() override;
	Entity& GetEntity(int index) { return Entity::register_.Get(entityIDs[index]); } // temp tool for GUI

};
