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


	template<typename T>
	std::string type_as_string(const T& obj)
	{
		return RemovePrefix(typeid(obj).name(), "class ");
	}
	template<typename T>
	std::string to_string()
	{
		return RemovePrefix(typeid(T).name(), "class ");
	}


	template<typename T>
	std::string to_string(std::vector<T>& vec)
	{
		std::string out = "";
		for (T t : vec)
			out += ", " + Tools::to_string(t);
		RemovePrefix(out, ", ");
		return "[" + out + "]";
	}

	// remove by index is simple myVector.erase(myVector.begin() + index);

	template<typename Tkey, typename Tval>
	std::string to_string(std::map<Tkey, Tval>& dict)
	{
		std::string out = "";
		for (const auto& pair : dict)
			out += "\n    " + Tools::to_string(pair.first) + ": " + Tools::to_string(pair.second);
		return "{" + out + "\n    }";
	}


}





