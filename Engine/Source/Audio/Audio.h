#pragma once
#include "miniaudio.h"
#include "ShortHands.h"
#include "logger.h"
#include <string>


namespace Audio
{
	Shorts;

	void Play(const string& filePath);
	ma_engine& Engine();;

};


namespace logger
{
	std::string to_string(ma_result result);
}
