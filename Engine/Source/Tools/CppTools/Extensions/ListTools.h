#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <sstream>
#include <functional>
#include "ErrorChecker.h"
#include "ShortHands.h"
#include "logger.h"

namespace Tools
{
	Shorts;
	// using vector find find_if map unordered_map
	//uint i = 0;

	template<typename T>
	bool Remove(vector<T>& vec, const T& t)
	{
		auto iterator = std::find(vec.begin(), vec.end(), t);
		if (iterator != vec.end())
		{
			vec.erase(iterator);
			return true;
		}
		return false;
	}

	template<typename T> 
	bool Remove(vector<T>& vec, std::reference_wrapper<T> t) 
	{
		auto iterator = std::find(vec.begin(), vec.end(), t);
		if (iterator != vec.end())
		{
			vec.erase(iterator);
			return true;
		}
		return false;
	}

	template< class T, class UnaryPredicate > 
	bool Remove(vector<T>& vec, const UnaryPredicate& condition)
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
	bool RemoveKey(map<Tkey, Tval>& dict, const Tkey& key)
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
	bool RemoveKey(unordered_map<Tkey, Tval>& dict, const Tkey& key)
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
	inline bool Contains(const vector<T>& vec, const T& t)
	{
		return std::find(std::begin(vec), std::end(vec), t) != std::end(vec);
	};

	
	template<typename Tkey, typename Tval> 
	bool ContainsKey(const map<Tkey, Tval>& dict, const Tkey& key)
	{
		return dict.find(key) != dict.end();
	};
	
	template<typename Tkey, typename Tval> 
	bool ContainsKey(const unordered_map<Tkey, Tval>& dict, const Tkey& key)
	{
		return dict.find(key) != dict.end();
	};
	

	template<typename Tkey, typename Tval> 
	vector<Tkey> GetKeys(const map<Tkey, Tval>& dict)
	{
		vector<Tkey> keys;
		for (const auto& pair : dict)
			keys.push_back(pair.first);
		return keys;
	};

	template<typename Tkey, typename Tval> 
	vector<Tkey> GetKeys(const unordered_map<Tkey, Tval>& dict)
	{
		vector<Tkey> keys;
		for (const auto& pair : dict)
			keys.push_back(pair.first);
		return keys;
	};
	
	template<typename Tkey, typename Tval>
	Tval at_default(const map<Tkey, Tval>& dict, const Tkey& key, const Tval& default_)
	{
		if (!Tools::ContainsKey(dict, key))
			return default_;
		return dict.at(key);
	};

	template<typename Tkey, typename Tval>
	Tval at_default(const unordered_map<Tkey, Tval>& dict, const Tkey& key, const Tval& default_)
	{
		if (!Tools::ContainsKey(dict, key))
			return default_;
		return dict.at(key);
	};

}

namespace logger
{
	Shorts;

	template <typename T>
	inline string to_string(const vector<T>& vec)
	{
		string out = "";
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
	inline string to_string(const map<Tkey, Tval>& dict)
	{
		string out = "\n";
		for (const auto& pair : dict)
			out += "{ " + logger::to_string(pair.first) + ": " + logger::to_string(pair.second) + " },\n";
		return "{ " + out + " }";
	};

	template<typename Tkey, typename Tval>
	inline string to_string(const unordered_map<Tkey, Tval>& dict)
	{
		string out = "\n";
		for (const auto& pair : dict)
		{
			out += "{ " + logger::to_string(pair.first) + ": " + logger::to_string(pair.second) + " },\n";
		}
		return "{ " + out + " }";
	};

	template<typename Tkey, typename Tval>
	inline string to_string(const pair<Tkey, Tval>& pair)
	{
		return "{ " + logger::to_string(pair.first) + ": " + logger::to_string(pair.second) + " }";
	};

}
