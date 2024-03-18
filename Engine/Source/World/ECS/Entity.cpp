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
	EntitiesByName[name].push_back(id); // [] initializes if the key is not present
}




uuids::uuid Entity::GetID(std::string name_)
{
	int count = Tools::ContainsKey_unordered(EntitiesByName, name_) ? EntitiesByName.at(name_).size() : 0;
	if (count != 1)
		RaiseError("There are " + std::to_string(count) + " entities named '" + name_
			+ "', so you cannot get an entity unambigously from this name."
			+ "\nEntitiesByName = \n" + logger::to_string(EntitiesByName));
	auto id = EntitiesByName.at(name_)[0];
	if (register_.Get(id).name != name_)
		RaiseError("Searched for an entity named " + name_ + " found one named " + register_.Get(id).name);

	return id;
}

uuids::uuid* Entity::TryGetID(std::string name_)
{
	int count = Tools::ContainsKey_unordered(EntitiesByName, name_) ? EntitiesByName.at(name_).size() : 0;
	if (count == 0)
		return nullptr;
	if (count > 1)
		RaiseError("There are " + std::to_string(count) + " entities named '" + name_
			+ "', so you cannot get an entity unambigously from this name."
			+ "\nEntitiesByName = \n" + logger::to_string(EntitiesByName));
	
	auto& id = EntitiesByName.at(name_)[0];
	if (register_.Get(id).name != name_)
		RaiseError("Searched for an entity named " + name_ + " found one named " + register_.Get(id).name);

	return &EntitiesByName.at(name_)[0]; // using id yields the warning "returning address of local variable or temporary"
}

std::string Entity::to_string() const
{
	Transform* transform = TryGetComponent<Transform>();
	if (transform == nullptr)
		return name;
	else
		return transform->GetPath();
}


void Entity::Update()
{
	#ifdef _DEBUG
	CheckConsistency();
	#endif // _DEBUG

	for (const auto& [e, components] : componentsByEntity)
	{
		for (const auto& compPtr : components)
			compPtr->OnUpdate();
	}
	DestroyTheDoomed();

}

void Entity::CheckConsistency()
{
	for (const auto& [name, entitiesWithName] : EntitiesByName)
	{
		if (entitiesWithName.size() != 1)
			Warning("entitiesWithName[", name, "] = ", entitiesWithName.size());
	}
	for (const auto& [entityID, components] : componentsByEntity)
	{
		Entity& entity = register_.Get(entityID);
		if (entity.GetID() != entityID)
			RaiseError("inconsistent data");
		EntitiesByName.at(entity.GetName()); // .at() fails if the key is not present. This constistutes a consistency check.
		for (const auto& compPtr : components)
			componentsByID.at(compPtr.get()->GetID()); // .at() fails if the key is not present. This constistutes a consistency check.
	}
	for (const auto& entity : register_.GetData())
		componentsByEntity.at(entity.GetID()); // .at() fails if the key is not present. This constistutes a consistency check.

}


// ---------------------------------- Destruction -------------------------------------


void Entity::DestroyTheDoomed()
{
	// callback
	for (const auto& [id_, comp] : componentsByID)
	{
		if (comp->isDoomed)
		{
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
	// destroy doomed components, that aren't on doomed entities
	for (const auto& [e, components] : componentsByEntity)
	{
		for (int i = components.size() - 1; i >= 0; i--)
		{
			if (components[i]->isDoomed)
				ClearData(components[i]);
		}
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
		if (EntitiesByName.at(name).size() == 1)
			EntitiesByName.erase(iterator2); // remove name and thereby also id
		else
			Tools::Remove(EntitiesByName.at(name), id); // remove id, but keep name
	}



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
