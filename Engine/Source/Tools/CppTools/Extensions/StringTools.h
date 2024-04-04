#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <string>

namespace Tools
{
	std::vector<std::string> SplitString(const std::string& input, const char* delimiter);
	std::vector<std::string> SplitString(const char* input, const char* delimiter);
	
	std::string Trim(const std::string& str);
	std::string Trim(const char* str);
	std::string RemovePrefix(const std::string& str, const std::string& prefix);
	std::string Replace(std::string str, const std::string& toBeReplaced, const std::string& replacement);

	// if used on A deriving from B, then this prints A. However, if used on a pointer to A that is declared as B*, this print B*.
	template<typename T>
	std::string TypeName(const T& obj)
	{
		return RemovePrefix(typeid(obj).name(), "class ");
	}
	template<typename T>
	std::string TypeName()
	{
		return RemovePrefix(typeid(T).name(), "class ");
	}


}





