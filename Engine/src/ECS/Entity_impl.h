#pragma once
#include "Entity.h"
#include "StringTools.h"



template <typename ComponentType>
ComponentType* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"GetComponent can only get components, not other types");

	//Log(name + ".Get: " + Tools::to_string(componentsByID[id]));
	for (const auto& c : componentsByID[id])
	{
		ComponentType* afterCast = dynamic_cast<ComponentType*>(c);
		if (afterCast != nullptr)
			return afterCast;
	}
	return nullptr;
}

template <typename ComponentType>
ComponentType* Entity::AddComponent()
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"AddComponent can only add components, not other types");
	//Log(name + ".Add1: " + Tools::to_string(componentsByID[id]));

	ComponentType* componentPtr = new ComponentType();
	componentsByID[id].push_back(componentPtr);
	componentPtr->InternalConstructor(id);

	//Log(name + ".Add2: " + Tools::to_string(componentsByID[id]));
	return componentPtr;
}


template <typename ComponentType> 
ComponentType& Entity::Get(uuids::uuid entityID)
{
	return *Entity::register_.Get(entityID).GetComponent<ComponentType>();
}

template <typename ComponentType> 
ComponentType& Entity::Add(uuids::uuid entityID)
{
	return *Entity::register_.Get(entityID).AddComponent<ComponentType>();
}








