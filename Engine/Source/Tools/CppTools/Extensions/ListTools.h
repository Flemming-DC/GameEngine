#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <sstream>
#include <functional>
#include "ErrorChecker.h"
#include "logger.h"

namespace Tools
{
	template<typename T>
	bool Remove(std::vector<T>& vec, T t)
	{
		auto iterator = std::find(vec.begin(), vec.end(), t);
		if (iterator != vec.end())
		{
			vec.erase(iterator);
			return true;
		}
		return false;
	}

	//template<typename T>
	template< class T, class UnaryPredicate >
	bool Remove(std::vector<T>& vec, const UnaryPredicate& condition)
	{
		auto iterator = std::find_if(vec.begin(), vec.end(), condition);
		if (iterator != vec.end())
		{
			vec.erase(iterator);
			return true;
		}
		return false;
	}

	// remove by index is simple myVector.erase(myVector.begin() + index);

	template<typename Tkey, typename Tval>
	bool RemoveKey(std::map<Tkey, Tval>& dict, Tkey key)
	{
		auto iterator = dict.find(key);
		if (iterator != dict.end())
		{
			dict.erase(iterator);
			return true;
		}
		return false;
	}

	template<typename Tkey, typename Tval>
	bool RemoveKey_unordered(std::unordered_map<Tkey, Tval>& dict, Tkey key)
	{
		auto iterator = dict.find(key);
		if (iterator != dict.end())
		{
			dict.erase(iterator);
			return true;
		}
		return false;
	}


	template<typename T>
	inline bool Contains(const std::vector<T>& vec, T t)
	{
		return std::find(std::begin(vec), std::end(vec), t) != std::end(vec);
	};

	
	template<typename Tkey, typename Tval>
	bool ContainsKey(const std::map<Tkey, Tval>& dict, Tkey key)
	{
		return dict.find(key) != dict.end();
	};
	
	template<typename Tkey, typename Tval>
	bool ContainsKey_unordered(const std::unordered_map<Tkey, Tval>& dict, Tkey key)
	{
		return dict.find(key) != dict.end();
	};
	

	template<typename Tkey, typename Tval>
	std::vector<Tkey> GetKeys(const std::map<Tkey, Tval>& dict)
	{
		std::vector<Tkey> keys;
		for (const auto& pair : dict)
			keys.push_back(pair.first);
		return keys;
	};

	template<typename Tkey, typename Tval>
	std::vector<Tkey> GetKeys_unordered(const std::unordered_map<Tkey, Tval>& dict)
	{
		std::vector<Tkey> keys;
		for (const auto& pair : dict)
			keys.push_back(pair.first);
		return keys;
	};

}

namespace logger
{

	template <typename T>
	inline std::string to_string(const std::vector<T>& vec)
	{
		std::string out = "";
		int count = vec.size();
		for (int i = 0; i < count; i++)
		{
			out += logger::to_string(vec[i]);
			if (i < count - 1)
				out += ", ";
		}
		return "{ " + out + " }";
	}

	template<typename Tkey, typename Tval>
	inline std::string to_string(const std::map<Tkey, Tval>& dict)
	{
		std::string out = "\n";
		for (const auto& pair : dict)
			out += "{ " + logger::to_string(pair.first) + ": " + logger::to_string(pair.second) + " },\n";
		return "{ " + out + " }";
	};

	template<typename Tkey, typename Tval>
	inline std::string to_string(const std::unordered_map<Tkey, Tval>& dict)
	{
		std::string out = "\n";
		for (const auto& pair : dict)
		{
			out += "{ " + logger::to_string(pair.first) + ": " + logger::to_string(pair.second) + " },\n";
		}
		return "{ " + out + " }";
	};

}
