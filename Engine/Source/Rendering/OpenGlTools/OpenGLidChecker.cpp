#include "OpenGLidChecker.h"
#include "OpenGlError.h"
#include "ListTools.h"
#include "StringTools.h"


Shorts;
map<string, vector<unsigned int>> OpenGLidChecker::openGLidByType = {};


void OpenGLidChecker::Add(const string& typeName, unsigned int openGLid)
{
	//string typeName = Tools::RemovePrefix(type.name(), "class ");
	if (!Tools::ContainsKey(openGLidByType, typeName))
		openGLidByType[typeName] = {};

	openGLidByType[typeName].push_back(openGLid);
}

void OpenGLidChecker::Remove(const string& typeName, unsigned int openGLid)
{
	Assert(Tools::ContainsKey(openGLidByType, typeName),
		"Trying to remove a typeName openGLid, but there are none.");

	Tools::Remove(openGLidByType[typeName], openGLid);
}

void OpenGLidChecker::CheckCleanup()
{
	for (const auto& pair : openGLidByType)
	{
		Assert(pair.second.empty(),
			"Some openGLids haven't been cleaned up.\n" 
			+ pair.first + " openGLids: " + logger::to_string(pair.second));
	}
}
