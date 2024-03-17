#pragma once
#include <string>
#include <memory>
#include "StringTools.h"
#include "GlmTools.h" // this contains some overloads of to_string that might be counterintuitively absent without this include
#include "ErrorChecker.h"

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

	template<typename T> string inline to_string(const T& value) { return value.to_string(); } // default to try and get a to_string method
	template<typename T> string inline to_string(T* value) { return "raw_ptr to " + (value ? to_string(*value) : "null"); } // raw pointer
	template<typename T> string inline to_string(std::unique_ptr<T> value) { return "unique_ptr to " + (value ? to_string(*value) : "null"); } // unique pointer
	template<typename T> string inline to_string(std::shared_ptr<T> value) { return "shared_ptr to " + (value ? to_string(*value) : "null"); } // shared pointer
	string inline to_string(const string& value) { return value; }
	string inline to_string(const char* value) { return string(value); }
	string inline to_string(const char value) { return string(&value); }
	string inline to_string(bool value) { return value ? "true" : "false"; }

	string inline to_string(int value) { return std::to_string(value); }
	string inline to_string(long long value) { return std::to_string(value); }
	string inline to_string(unsigned int value) { return std::to_string(value); }
	string inline to_string(unsigned long value) { return std::to_string(value); }
	string inline to_string(unsigned long long value) { return std::to_string(value); }
	string inline to_string(float value) { return std::to_string(value); }
	string inline to_string(double value) { return std::to_string(value); }
	string inline to_string(long double value) { return std::to_string(value); }

	
	
}








