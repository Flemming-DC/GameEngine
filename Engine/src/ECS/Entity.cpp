#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"

std::unordered_map<uuids::uuid, std::vector<Component*>> Entity::componentsByID;
//std::unordered_map<const Entity*, std::vector<Component*>> Entity::componentsByID;
//std::unordered_map<uuids::uuid, Entity&> Entity::entitiesByID;
Register<Entity> Entity::register_;

Entity::Entity(std::string name) : name(name), id(UuidCreator::MakeID())
{
	Log("Entity: " + name + ", " + UuidCreator::to_string(id));
	componentsByID[id] = {};
	//entitiesByID[id] = *this;
}

void Entity::Destroy() // Entity::~Entity()
{
	Log("Entity.Destroy " + name);
	for (const auto& c : componentsByID[id])
	{
		c->entityIsDoingcleanup = true;
		delete c;
	}

	// here we remove the key. The functions in the toolbox do not help, do to some junk "features" about const
	auto iterator = componentsByID.find(id);
	if (iterator != componentsByID.end())
		componentsByID.erase(iterator);

}

void Entity::UpdateAllEntities()
{
	for (const auto& pair : componentsByID)
	{
		Log(Entity::register_.Get(pair.first).name);
		Log(Tools::to_string(pair.second));
		for (const auto& c : pair.second)
		{
			Log("    " + c->to_string());
			c->OnUpdate();
		}
	}
}



uuids::uuid Make(std::string name = "Entity")
{
	return Entity::register_.Add(name).GetID();
}