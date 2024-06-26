#include "Naming.h"
#include "StringTools.h"
#include "ImGuiTools.h"
#include "UuidCreator.h"
#include "YmlTools.h"
#include "EngineLiterals.h"
#include <regex>
Shorts;

void Naming::AddWithSuffix(string& name, uuid id)
{
	if (Tools::ContainsKey(idByName, name))
	{
		std::regex pattern(" \\(\\d+\\)$"); // Matches " (digits)" at the end of the string
		auto nameWithoutSuffix = std::regex_replace(name, pattern, "");

		// get maxNumberSuffix
		int maxNumberSuffix = 0;
		for (const auto& [name_, id_] : idByName)
		{
			// try find suffix " (digits)". If it isn't there, then continue
			vector<string> split = Tools::SplitString(name_, " (");
			if (split.size() == 1)
				continue; // if there is no suffix " (digits)" then continue
			string numberStr = Tools::RemoveSuffix(split.back(), ")");
			bool isDigits = true;
			for (char c : numberStr)
			{
				if (!std::isdigit(c))
					isDigits = false;
			}
			if (!isDigits)
				continue; // if there is no suffix " (digits)" then continue
			int numberSuffix = std::stoi(numberStr);
			
			// numberSuffix found
			if (numberSuffix > maxNumberSuffix)
				maxNumberSuffix = numberSuffix;
		}
		
		int newNumberSuffix = maxNumberSuffix + 1;
		name = logger::make_string(nameWithoutSuffix, " (", newNumberSuffix, ")");
	}

	idByName[name] = id;
}

bool Naming::TryAdd(const string& name, uuid id)
{
	for (const auto& [name_, id_] : idByName)
	{
		if (name_ == name || id_ == id)
			return false;
	}
	idByName[name] = id;
	return true;
}

void Naming::Add(const string& name, uuid id)
{
	for (const auto& [name_, id_] : idByName)
	{
		Deny(name_ == name,
			"This name ", name, " is already used in Naming ", idByName);
		Deny(id_ == id,
			"This id ", id, " is already named in Naming ", idByName);
	}
	idByName[name] = id;
}

void Naming::Remove(const string& name)
{
	Tools::RemoveKey(idByName, name);
}

string Naming::at(uuid id)
{
	for (const auto& [name_, id_] : idByName)
	{
		if (id_ == id)
			return name_;
	}
	RaiseError("Failed to find id ", id, " in Naming ", idByName);
}

bool Naming::Contains(uuid id) const
{
	for (const auto& [name_, id_] : idByName)
	{
		if (id_ == id)
			return true;
	}
	return false;
}
bool Naming::Contains(string name) const
{
	return Tools::ContainsKey(idByName, name);
}

optional<uuid> Naming::TryGet(string name)
{
	for (const auto& [name_, id_] : idByName)
	{
		if (name_ == name)
			return id_;
	}
	return std::nullopt;
}
optional<string> Naming::TryGet(uuid id)
{
	for (const auto& [name_, id_] : idByName)
	{
		if (id_ == id)
			return name_;
	}
	return std::nullopt;
}


optional<uuid> Naming::Show(const char* label, uuid currentID)
{
	vector<string>& names = Names();
	string name = at(currentID);
	optional<int> currentIndex = Tools::FindIndex(names, name);
	Assert(currentIndex, "Failed to find index");

	//static int currentIndex = 0;
	bool changed = ImGui::Combo(label, &*currentIndex, names);
	if (changed && !names.empty())
		return at(names[*currentIndex]);
	else
		return std::nullopt;
}

void Naming::Save(string fileName)
{
	Node node;
	for (auto& [name, id] : idByName)
		node[name] = id;
	YmlTools::Save(node, Literals::Namings + fileName, true, false);
}

void Naming::Load(string fileName)
{
	Node node = YmlTools::Load(Literals::Namings + fileName, true);
	if (node.IsNull())
		return;
	auto idByName_ = node.as<map<string, uuid>>();
	for (auto& [name, id] : idByName_)
	{
		bool wasAdded = TryAdd(name, id); // O(n^2) growth
		if (!wasAdded)
			Warning("Failed to add ", name, " to naming. It or its id is already present.");
	}
	/*
	* // unsafe, but linear growth
	idByName.clear();
	auto idByName_ = node.as<map<string, uuid>>();
	for (auto& [name, id] : idByName_)
		idByName[name] = id;
	*/
}



string Naming::to_string() const 
{ 
	return logger::to_string(idByName); 
}