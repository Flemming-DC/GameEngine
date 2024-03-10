#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"
#include "Transform.h"


std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> Entity::componentsByEntity;
std::unordered_map<std::string, std::vector<uuids::uuid>> Entity::EntitiesByName;
std::unordered_map<uuids::uuid, Component*> Entity::componentsByID;
Register<Entity> Entity::register_;



Entity::Entity(std::string name, uuids::uuid* id_) : name(name)
{
	id = id_ == nullptr ? UuidCreator::MakeID() : *id_; // you get an input id_ iff it is loaded as an asset.
	if (!Tools::ContainsKey_unordered(EntitiesByName, name))
		EntitiesByName[name] = {};
	EntitiesByName[name].push_back(id);
}

// currently, this just removes all data on the entity, turning it into a blank entity, but it leaves a blank entity behind
void Entity::Destroy() // Entity::~Entity()
{
	if (isDestroyed)
		return;

	// destroy children too
	Transform* transform = TryGetComponent<Transform>();
	for (const auto& child : transform->GetChildren())
		child->GetEntity().Destroy();

	// remove and thereby destroy components on the entity
	for (int i = componentsByEntity[id].size() - 1; i>=0; i--)
		RemoveComponent(*componentsByEntity[id][i]);

	// here we remove the entity from this dict. The functions in the toolbox do not help, due to some junk "features" about const
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
	
	isDestroyed = true;
	// Entity::register_.Remove(id); // <------- take care of this
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



bool Entity::RemoveComponent(const Component& comp)
{
	for (const auto& compPtr : componentsByEntity[id])
	{
		if (compPtr->GetID() != comp.GetID())
			continue;
		compPtr->OnDestroy();
		auto iterator = std::find(componentsByEntity[id].begin(), componentsByEntity[id].end(), compPtr);
		if (iterator != componentsByEntity[id].end())
			componentsByEntity[id].erase(iterator);
		Tools::RemoveKey_unordered(componentsByID, comp.GetID());
		return true;
	}
	return false;
}
