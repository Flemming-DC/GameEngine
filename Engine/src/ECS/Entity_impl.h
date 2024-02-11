#pragma once
#include "Entity.h"



template <typename ComponentType>
ComponentType* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"GetComponent can only get components, not other types");

	for (const auto& c : componentsByID[this])
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

	ComponentType* componentPtr = new ComponentType();
	componentsByID[this].push_back(componentPtr);
	componentPtr->InternalConstructor(this);

	return componentPtr;
}
