#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"

std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> Entity::componentsByEntity;
std::unordered_map<std::string, std::vector<uuids::uuid>> Entity::EntitiesByName;
Register<Entity> Entity::register_;



Entity::Entity(std::string name, uuids::uuid* id_) : name(name)
{
	id = id_ == nullptr ? UuidCreator::MakeID() : *id_; // you get an input id_ iff it is loaded as an asset.
	if (!Tools::ContainsKey_unordered(EntitiesByName, name))
		EntitiesByName[name] = {};
	EntitiesByName[name].push_back(id);
}

void Entity::Destroy() // Entity::~Entity()
{
	Log("Entity.Destroy " + name);
	/*
	for (const auto& c : componentsByEntity[id])
		c->entityIsDoingcleanup = true;
	*/

	// here we remove the entity. The functions in the toolbox do not help, due to some junk "features" about const
	auto iterator1 = componentsByEntity.find(id);
	if (iterator1 != componentsByEntity.end())
		componentsByEntity.erase(iterator1);
	
	// next we remove this entity from the EntitiesByName.
	auto iterator2 = EntitiesByName.find(name);
	if (iterator2 != EntitiesByName.end())
	{
		if (EntitiesByName[name].size() == 1)
			EntitiesByName.erase(iterator2); // remove name and thereby also id
		else
			Tools::Remove(EntitiesByName[name], id); // remove id, but keep name
	}
	
	// Entity::register_.Remove(id); <------- take care of this
}

void Entity::UpdateAllEntities()
{
	for (const auto& pair : componentsByEntity)
	{
		for (const auto& c : pair.second)
			c->OnUpdate();
	}
}



uuids::uuid Entity::Make(std::string name)
{
	return Entity::register_.Add(name).GetID();
}



uuids::uuid Entity::GetID(std::string name_)
{
	int count = EntitiesByName[name_].size();
	if (count != 1)
		RaiseError("There are " + std::to_string(count) + " entities named " + name_
			+ ", so you cannot get an entity unambigously from this name.");
	auto id = EntitiesByName[name_][0];
	if (register_.Get(id).name != name_)
		RaiseError("Search for an entity named " + name_ + " found one named " + register_.Get(id).name);

	return id;
}
