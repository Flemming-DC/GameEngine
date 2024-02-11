#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

namespace Tools
{
	std::vector<std::string> SplitString(const std::string& input, const char* delimiter);
	std::vector<std::string> SplitString(const char* input, const char* delimiter);
	
	std::string Trim(const std::string& str);
	std::string Trim(const char* str);


}





