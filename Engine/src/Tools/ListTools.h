#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <sstream>
#include <ErrorChecker.h>


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
	bool Contains(const std::vector<T>& vec, T t)
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
	std::vector<Tkey> GetKeys_unordered(const std::unordered_map<Tkey, Tval>& dict)
	{
		std::vector<Tkey> keys;
		for (const auto& pair : dict)
			keys.push_back(pair.first);
		return keys;
	};

	template<typename Tkey, typename Tval>
	std::vector<Tkey> GetKeys(const std::map<Tkey, Tval>& dict)
	{
		std::vector<Tkey> keys;
		for (const auto& pair : dict)
			keys.push_back(pair.first);
		return keys;
	};

	template <typename T>
	std::string to_string(const std::vector<T>& vec)
	{
		std::ostringstream result;
		for (int i=0; i<(int)vec.size(); i++)
		{
			try
			{
				result << vec[i];
				if (i < (int)vec.size() - 1)
					result << ", ";
			}
			catch (const std::exception& _)
			{
				(void)_; // eliminate unused warning
				RaiseError("Failed to pipe element " + std::to_string(i) + " of vector into ostringstream");
			}

		}
		
		return "{ " + result.str() + " }";
	}

}
