#pragma once
#include "Entity.h"
#include "StringTools.h"



template <typename ComponentType>
ComponentType* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"GetComponent can only get components, not other types");

	for (const auto& c : componentsByEntity[id])
	{
		ComponentType* afterCast = dynamic_cast<ComponentType*>(c.get());
		if (afterCast == nullptr)
			continue;
		return afterCast;
	}
	return nullptr;
}

template <typename ComponentType>
ComponentType* Entity::AddComponent()
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"AddComponent can only add components, not other types");
	
	componentsByEntity[id].push_back(std::make_unique<ComponentType>());
	Component* ptr = componentsByEntity[id].back().get();
	ComponentType* afterCast = dynamic_cast<ComponentType*>(ptr);
	if (afterCast == nullptr)
		RaiseError("dynamic_cast failed for " + name + ".AddComponent<" + Tools::to_string<ComponentType>() + ">()");
	afterCast->InternalConstructor(id);
	return afterCast;

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








