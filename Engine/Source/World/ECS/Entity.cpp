#include "Entity.h"
#include "Component.h"
#include <ListTools.h>
#include "UuidCreator.h"
#include "Transform.h"
#include "Delay.h"

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

std::string Entity::to_string() const
{
	Transform* transform = TryGetComponent<Transform>();
	if (transform == nullptr)
		return name;
	else
		return transform->GetPath();
}


// ---------------------------------- Destruction -------------------------------------


// currently, this just removes all data on the entity, turning it into a blank entity, but it leaves a blank entity behind
void Entity::Destroy() // Entity::~Entity()
{
	isDoomed = true;

	// doom children
	Transform* transform = TryGetComponent<Transform>();
	for (const auto& child : transform->GetChildren())
		child->GetEntity().Destroy();

	// doom components
	for (const auto& comp : GetComponents())
		Destroy(comp.get());

}

void Entity::ClearData()
{

	// destroy components on the entity
	for (int i = componentsByEntity[id].size() - 1; i >= 0; i--)
		ClearData(componentsByEntity[id][i]);

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

}

void Entity::Update()
{
	for (const auto& [e, components] : componentsByEntity)
	{
		for (const auto& c : components)
			c->OnUpdate();
	}
	DestroyTheDoomed();

}

void Entity::DestroyTheDoomed()
{
	// callback
	for (const auto& [id_, comp] : componentsByID)
	{
		if (comp->isDoomed)
		{
			P(comp);
			comp->OnDestroy();
		}
	}
	// get ids
	std::vector<uuids::uuid> doomedEntityIDs;
	for (Entity& entity : register_.GetData())
	{
		if (entity.isDoomed)
			doomedEntityIDs.push_back(entity.GetID());
	}
	// destroy doomed entities
	for (const auto& entityID : doomedEntityIDs)
		Entity::register_.Get(entityID).ClearData();
	for (const auto& entityID : doomedEntityIDs)
		Entity::register_.Remove(entityID);
	// destroy doomed components
	for (const auto& [e, components] : componentsByEntity)
	{
		for (int i = components.size() - 1; i >= 0; i--)
		{
			if (components[i]->isDoomed)
			{
				P(*components[i]);
				ClearData(components[i]);
			}
		}
		/*
		for (const auto& comp : components)
		{
			if (comp->isDoomed)
				ClearData(comp);
		}
		*/
	}

}

void Entity::DestroyEverything()
{
	for (const auto& [id_, comp] : componentsByID)
		comp->OnDestroy();
	
	EntitiesByName.clear();
	componentsByID.clear();
	componentsByEntity.clear();
	register_.Clear();
}



bool Entity::Destroy(Component* comp)
{
	if (!comp)
		return false;
	for (const auto& compPtr : componentsByEntity[id])
	{
		if (compPtr->GetID() != comp->GetID())
			continue;
		comp->isDoomed = true;
		return true;
	}
	return false;
}

void Entity::ClearData(const std::unique_ptr<Component>& compPtr)
{
	auto& entityID = compPtr->GetEntity().GetID();

	Tools::RemoveKey_unordered(componentsByID, compPtr->GetID());

	auto iterator = std::find(componentsByEntity[entityID].begin(), componentsByEntity[entityID].end(), compPtr);
	if (iterator != componentsByEntity[entityID].end())
		componentsByEntity[entityID].erase(iterator);
	
}
