#pragma once
#include <string>
#include <memory>
#include "StringTools.h"
#include "GlmTools.h" // this contains some overloads of to_string that might be counterintuitively absent without this include


namespace logger
{
	using std::string;

	template<typename... Args>
	inline void print(Args... args)
	{
		string message = make_string(args...);
		std::cout << message << std::endl;;
	}

	template<typename T, typename... Args>
	string inline make_string(T first, Args... args)
	{
		string out = to_string(first);
		out += make_string(args...); 
		return out;
	}
	string inline make_string() { return ""; } // handle end of recursion


	string inline file_name(std::string path)
	{
		string file_name = Tools::SplitString(path, "\\").back(); // works on windows which uses folder\file.ext
		file_name = Tools::SplitString(file_name, "/").back(); // works on those who use folder/file.ext
		file_name = Tools::SplitString(file_name, ".")[0]; // remove extension
		return file_name;
	}


	// --------------------- overloads of to_string ---------------------

	template<typename T> string inline to_string(const T& value) { return std::to_string(value); } // default to std::to_string if nothing else is available
	template<typename T> string inline to_string(T* value) { return "raw_ptr " + to_string(*value); } // raw pointer
	template<typename T> string inline to_string(std::unique_ptr<T> value) { return "unique_ptr " + to_string(*value); } // unique pointer
	template<typename T> string inline to_string(std::shared_ptr<T> value) { return "shared_ptr " + to_string(*value); } // shared pointer
	string inline to_string(const string& value) { return value; }
	string inline to_string(const char* value) { return string(value); }
	string inline to_string(bool value) { return value ? "true" : "false"; }


	/*
	template <typename T>
	std::string to_string(const std::vector<T>& vec)
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
	*/
}







