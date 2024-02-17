#pragma once
#include <map>
#include <vector>
#include <string>
#include <typeinfo>
#include "StringTools.h" // we include #include, since its used by anything that uses OpenGLidChecker


class OpenGLidChecker
{
public:
	static void Add(const std::string& typeName, unsigned int openGLid);
	static void Remove(const std::string& typeName, unsigned int openGLid);
	static void CheckCleanup();

private:
	static std::map<std::string, std::vector<unsigned int>> openGLidByType;
};

