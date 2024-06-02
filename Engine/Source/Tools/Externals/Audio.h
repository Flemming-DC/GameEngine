#pragma once
#include "miniaudio.h"
#include "ShortHands.h"
#include <string>


namespace Audio
{
	Shorts;

	void Setup();
	void Play(const string& filePath, optional<vec2> position);
	void ShutDown();
};



