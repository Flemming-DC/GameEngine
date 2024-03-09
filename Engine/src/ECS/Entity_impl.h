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
		RaiseError("Failed to find " + Tools::to_string<ComponentType>() + " on " + name);
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
		RaiseError("dynamic_cast failed for " + name + ".AddComponent<" + Tools::to_string<ComponentType>() + ">()");
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
		RaiseError("Failed to find " + Tools::to_string<ComponentType>()
			+ " with id " + UuidCreator::to_string(id_));
	return *afterCast;
}

template <typename ComponentType>
ComponentType* Entity::TryGetComponent(uuids::uuid id_)
{
	ComponentType* afterCast = dynamic_cast<ComponentType*>(componentsByID[id_]);
	return afterCast;
}

/*
template <typename ComponentType>
bool Entity::RemoveComponent(ComponentType comp)
{
	auto compPtr = TryGet<ComponentType>();
	if (compPtr == nullptr)
		return false;
	Tools::RemoveKey_unordered(componentsByID, compPtr->GetID());

	for (const auto& _compPtr : componentsByEntity[id])
	{
		if (_compPtr->GetID() == compPtr->GetID())
		{
			Tools::Remove(componentsByEntity[id], _compPtr); // loop cannot continue after remove
			return true;
		}
	}
	
}
*/



