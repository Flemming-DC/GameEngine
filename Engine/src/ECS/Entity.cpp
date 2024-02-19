#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"

std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> Entity::componentsByEntity;
Register<Entity> Entity::register_;


Entity::Entity(std::string name) : name(name), id(UuidCreator::MakeID())
{
	Log("Entity: " + name + ", " + UuidCreator::to_string(id));
}

void Entity::Destroy() // Entity::~Entity()
{
	Log("Entity.Destroy " + name);
	/*
	for (const auto& c : componentsByEntity[id])
		c->entityIsDoingcleanup = true;
	*/

	// here we remove the entity. The functions in the toolbox do not help, due to some junk "features" about const
	auto iterator = componentsByEntity.find(id);
	if (iterator != componentsByEntity.end())
		componentsByEntity.erase(iterator);

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