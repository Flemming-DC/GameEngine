#pragma once
#include "Entity.h"
#include "StringTools.h"



template <typename ComponentType>
ComponentType& Entity::GetComponent() const
{
	ComponentType* comp = TryGetComponent<ComponentType>();
	if (comp)
		return *comp;
	else
		RaiseError("Failed to find " + Tools::TypeName<ComponentType>() + " on " + name);
}


template <typename ComponentType>
ComponentType* Entity::TryGetComponent() const
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"GetComponent can only get components, not other types");

	for (const auto& comp : componentsByEntity[id])
	{
		ComponentType* afterCast = dynamic_cast<ComponentType*>(comp.get());
		if (afterCast == nullptr)
			continue;
		return afterCast;
	}
	return nullptr;
}

template <typename ComponentType>
ComponentType& Entity::AddComponent(YAML::Node* node)
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"AddComponent can only add components, not other types");
	
	componentsByEntity[id].push_back(std::make_unique<ComponentType>());
	Component* ptr = componentsByEntity[id].back().get();
	ComponentType* afterCast = dynamic_cast<ComponentType*>(ptr);
	if (afterCast == nullptr)
		RaiseError("dynamic_cast failed for " + name + ".AddComponent<" + Tools::TypeName<ComponentType>() + ">()");
	afterCast->OnAddComponent(id, node);
	componentsByID[afterCast->id] = ptr;
	return *afterCast;

}


template <typename ComponentType> 
ComponentType& Entity::LoadComponent(YAML::Node& node)
{
	return AddComponent<ComponentType>(&node);
}


template <typename ComponentType>
ComponentType* Entity::TryGet(uuids::uuid entityID)
{
	return Entity::register_.Get(entityID).TryGetComponent<ComponentType>();
}

template <typename ComponentType> 
ComponentType& Entity::Get(uuids::uuid entityID)
{
	return Entity::register_.Get(entityID).GetComponent<ComponentType>();
}

template <typename ComponentType> 
ComponentType& Entity::Add(uuids::uuid entityID)
{
	return Entity::register_.Get(entityID).AddComponent<ComponentType>();
}


template <typename ComponentType>
ComponentType& Entity::GetComponent(uuids::uuid id_)
{
	ComponentType* afterCast = dynamic_cast<ComponentType*>(componentsByID[id_]);
	if (!afterCast)
		RaiseError("Failed to find " + Tools::TypeName<ComponentType>()
			+ " with id " + logger::to_string(id_));
	return *afterCast;
}

template <typename ComponentType>
ComponentType* Entity::TryGetComponent(uuids::uuid id_)
{
	ComponentType* afterCast = dynamic_cast<ComponentType*>(componentsByID[id_]);
	return afterCast;
}



