#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"

std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> Entity::componentsByID_unique;
Register<Entity> Entity::register_;


Entity::Entity(std::string name) : name(name), id(UuidCreator::MakeID())
{
	Log("Entity: " + name + ", " + UuidCreator::to_string(id));
	//componentsByID[id] = {};
}

void Entity::Destroy() // Entity::~Entity()
{
	Log("Entity.Destroy " + name);
	for (const auto& c : componentsByID_unique[id])
	{
		c->entityIsDoingcleanup = true;
		//delete c;
	}

	// here we remove the entity. The functions in the toolbox do not help, due to some junk "features" about const
	auto iterator = componentsByID_unique.find(id);
	if (iterator != componentsByID_unique.end())
		componentsByID_unique.erase(iterator);

}

void Entity::UpdateAllEntities()
{
	for (const auto& pair : componentsByID_unique)
	{
		for (const auto& c : pair.second)
			c->OnUpdate();
	}
}



uuids::uuid Entity::Make(std::string name)
{
	return Entity::register_.Add(name).GetID();
}