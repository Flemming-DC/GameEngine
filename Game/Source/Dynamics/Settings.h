#pragma once
#include "Dynamic.h"

class Settings : public Dynamic
{
private:
	void OnGameStart() override;
};

