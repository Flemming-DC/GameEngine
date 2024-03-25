#pragma once
#include <string>
#include <iostream>
#include "StringTools.h"

#ifdef _DEBUG
#define P(...) logger::print(logger::file_name(__FILE__), ".", __func__, ": ", __VA_ARGS__);
#define PP P("")
#endif // _DEBUG


namespace logger
{
	template<typename... Args> void inline print(Args... args);
	template<typename T, typename... Args> std::string inline make_string(T first, Args... args);
	template<typename T> std::string inline to_string(const T& value);
	inline std::string file_name(std::string path);


	// ---------------- check if object has to_string method ---------------



}

#include "logger_impl.h"




