#pragma once
#include "Entity.h"
#include "StringTools.h"



template <typename ComponentType>
ComponentType* Entity::GetComponent() const
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"GetComponent can only get components, not other types");

	//Log(name + ".Get: " + Tools::to_string(componentsByID[id]));
	/*for (const auto& c : componentsByID[id])
	{
		ComponentType* afterCast = dynamic_cast<ComponentType*>(c); // .get()
		if (afterCast != nullptr)
			return afterCast;
	}*/
	for (const auto& c : componentsByID_unique[id])
	{
		//auto& refComp = *c;
		//ComponentType& afterCastRef = dynamic_cast<ComponentType&>(refComp); // breaks for RectangleCollider, but not Transform
		

		Component* ptr = c.get();
		//Log("Entity::GetComponent 1: " + ptr->to_string() + "." + Tools::to_string<ComponentType>() 
		//	+ " is " + Tools::to_string(*ptr));
		ComponentType* afterCast = dynamic_cast<ComponentType*>(ptr); // breaks for RectangleCollider, but not Transform
		if (afterCast == nullptr)
		{
			continue;
			//RaiseError("dynamic_cast on pointer failed: " + ptr->to_string() + "." + Tools::to_string<ComponentType>());
		}
		return afterCast;
	}
	return nullptr;
}

template <typename ComponentType>
ComponentType* Entity::AddComponent()
{
	static_assert(std::is_base_of<Component, ComponentType>::value,
		"AddComponent can only add components, not other types");
	
	componentsByID_unique[id].push_back(std::make_unique<ComponentType>());
	Component* ptr = componentsByID_unique[id].back().get();
	ComponentType* afterCast = dynamic_cast<ComponentType*>(ptr);
	if (afterCast == nullptr)
		RaiseError("AddComponent dynamic_cast failed");
	//Log("Entity::Add 2: " + afterCast->to_string() + "." + Tools::to_string<ComponentType>()
	//	+ " is " + Tools::to_string(afterCast));
	//Log("Entity::AddComponent 2");
	afterCast->InternalConstructor(id);
	//Log("Entity::AddComponent 3");
	return afterCast;

	/*
	ComponentType* componentPtr = new ComponentType();
	componentsByID[id].push_back(componentPtr);
	componentPtr->InternalConstructor(id);
	return componentPtr;
	*/
	
	/*
	std::unique_ptr<ComponentType> componentPtr = std::make_unique<ComponentType>();
	componentsByID[id].push_back(std::move(componentPtr));
	componentPtr->InternalConstructor(id);
	//Log(name + ".Add2: " + Tools::to_string(componentsByID[id]));
	return componentPtr.get();
	*/
	/*
	componentsByID[id].emplace_back(std::make_unique<ComponentType>());
	componentsByID[id].back()->InternalConstructor(id);

	ComponentType* afterCast = dynamic_cast<ComponentType*>(componentsByID[id].back().get());
	if (afterCast == nullptr)
		RaiseError("dynamic_cast failed");
	return afterCast;
	*/
}
/*
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
*/
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








