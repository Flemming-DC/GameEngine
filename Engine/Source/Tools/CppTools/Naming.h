#pragma once
#include "ListTools.h"

class Naming
{
	Shorts
public:
	void AddWithSuffix(string& name, uuid id); // gives name a suffix " (number)" to avoid naming collision
	bool TryAdd(const string& name, uuid id);
	void Add(const string& name, uuid id);
	void Remove(const string& name);
	inline uuid at(string name) { return idByName.at(name); }
	string at(uuid name);
	
	vector<string> Names() { return Tools::GetKeys(idByName); } // copying
	optional<uuid> Show(const char* label, uuid currentID); // returns the new uuid, if it differs from the old

private:
	map_uo<string, uuid> idByName;

};

