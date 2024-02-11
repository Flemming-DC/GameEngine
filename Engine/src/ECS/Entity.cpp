#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"

std::unordered_map<const Entity*, std::vector<Component*>> Entity::componentsByID;

Entity::Entity(std::string name) : name(name), id(UuidCreator::MakeUUID()){ }

Entity::~Entity()
{
	for (const auto& c : componentsByID[this])
	{
		c->entityIsDoingcleanup = true;
		delete c;
	}

	// here we remove the key. The functions in the toolbox do not help, do to some junk "features" about const
	auto iterator = componentsByID.find(this);
	if (iterator != componentsByID.end())
		componentsByID.erase(iterator);

}

void Entity::UpdateAllEntities()
{
	for (const auto& pair : componentsByID)
	{
		for (const auto& c : pair.second)
			c->OnUpdate();
	}
}
