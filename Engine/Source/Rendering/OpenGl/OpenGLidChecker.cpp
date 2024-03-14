#include "OpenGLidChecker.h"
#include "OpenGlError.h"
#include "ListTools.h"
#include "StringTools.h"


using namespace std;
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
	if (!Tools::ContainsKey(openGLidByType, typeName))
		RaiseError("Trying to remove a typeName openGLid, but there are none.");

	Tools::Remove(openGLidByType[typeName], openGLid);
}

void OpenGLidChecker::CheckCleanup()
{
	for (const auto& pair : openGLidByType)
	{
		if (pair.second.size() != 0)
			RaiseError("Some openGLids haven't been cleaned up.\n" 
				+ pair.first + " openGLids: " + logger::to_string(pair.second));
	}
}
