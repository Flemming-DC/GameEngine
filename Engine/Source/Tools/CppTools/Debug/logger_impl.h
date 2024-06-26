#pragma once
#include <string>
#include <memory>
#include <optional>
#include <functional>
#include "StringTools.h"
#include "GlmTools.h" // this contains some overloads of to_string that might otherwise be counterintuitively absent
#include "ListTools.h" // this contains some overloads of to_string that might otherwise be counterintuitively absent
#include "YmlTools.h"

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
		out += _internal_make_string(args...);
		return out;
	}

	template<typename T, typename... Args>
	string inline _internal_make_string(T first, Args... args)
	{
		string out = to_string(first);
		out += _internal_make_string(args...);
		return out;
	}
	string inline _internal_make_string() { return ""; } // handle end of recursion


	string inline file_name(string path)
	{
		string file_name = Tools::SplitString(path, "\\").back(); // works on windows which uses folder\file.ext
		file_name = Tools::SplitString(file_name, "/").back(); // works on those who use folder/file.ext
		file_name = Tools::SplitString(file_name, ".")[0]; // remove extension
		return file_name;
	}



	// ---------------- check if object has to_string method ---------------

	// SFINAE test
	template <typename... > using 
	void_t = void;

	template <typename T, typename = void>
	struct has_to_string_const_method : std::false_type {};

	template <typename T>
	struct has_to_string_const_method<T, void_t<
		decltype(std::declval<const T&>().to_string())
		>> : std::true_type {};


	template<typename T>
	typename std::enable_if<has_to_string_const_method<T>::value, std::string>::type
		TryCallToString(const std::type_info& type_, const T& t)
	{
		return t.to_string();
	}

	std::string TryCallToString(const std::type_info& type_, ...); // implemented in cpp


	// ---------------- overloads of to_string ---------------



	template<typename T> string inline to_string(const T& value) { return TryCallToString(typeid(T), value); } // default to try and get a to_string method
	template<typename T> string inline to_string(T* value) { return "raw_ptr to " + (value ? to_string(*value) : "null"); } // raw pointer
	template<typename T> string inline to_string(std::unique_ptr<T> value) { return "unique_ptr to " + (value ? to_string(*value) : "null"); } // unique pointer
	template<typename T> string inline to_string(std::shared_ptr<T> value) { return "shared_ptr to " + (value ? to_string(*value) : "null"); } // shared pointer
	template<typename T> string inline to_string(std::optional<T> value) { return "optional " + (value ? to_string(*value) : "null"); } // optional
	
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

	template<typename T, typename... Args> string inline to_string(std::function<T(Args...)> value)
	{
		string out = Tools::TypeName(value);
		out = Tools::RemovePrefix(out, "std::function");
		out = Tools::Replace(out, " __cdecl", " ");
		out = Tools::Replace(out, ",class ", ", ");
		out = Tools::Replace(out, "(class ", "(");
		return "function " + out;
	}

	string inline to_string(YAML::Node node)
	{
		YAML::Emitter emitter;
		emitter.SetIndent(4);
		emitter.SetSeqFormat(YAML::Flow); // write lists horizontally, not vertically
		emitter << node;
		return string("Node:\n") + emitter.c_str();
	}

	/*
	template<typename EnumType>
	typename std::enable_if<std::is_enum<EnumType>::value, string>::type
		TryCallToString(const std::type_info& type_, const EnumType& e)
	{
		string out = "";
		//  RemovePrefix(type_.name(), "class ") 
		out += Tools::TypeName<EnumType>() + " value " + std::to_string((int)e);
		return out;
	}*/
	
}








