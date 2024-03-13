#pragma once
#include <string>
#include <iostream>
#include "StringTools.h"

#ifdef _DEBUG
#define P(...) logger::print(logger::file_name(__FILE__), ".", __func__, ": ", __VA_ARGS__);
#endif // _DEBUG


namespace logger
{
	template<typename... Args> void inline print(Args... args);
	template<typename T, typename... Args> std::string inline make_string(T first, Args... args);
	template<typename T> std::string inline to_string(const T& value);
	inline std::string file_name(std::string path);




	/*

	template<typename T>
	std::string to_string(const T& value)
	{
		// Forward to std::to_string
		return std::to_string(value);
	}

	std::string to_string(const std::string& value)
	{
		return value;
	}
	std::string to_string(const char* value)
	{
		return std::string(value);
	}
	*/
}

#include "logger_impl.h"




