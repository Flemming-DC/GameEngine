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

	if (!Tools::ContainsKey(componentsByEntity, id))
		return nullptr;
	for (const auto& comp : componentsByEntity.at(id))
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
	bool hasComponent = (TryGet<ComponentType>() != nullptr);
	
	componentsByEntity[id].push_back(std::make_unique<ComponentType>());
	Component* ptr = componentsByEntity[id].back().get();
	ComponentType* afterCast = dynamic_cast<ComponentType*>(ptr);
	if (afterCast == nullptr)
		RaiseError("dynamic_cast failed for " + name + ".AddComponent<" + Tools::TypeName<ComponentType>() + ">()");
	afterCast->OnAddComponent(id, node);
	componentByID[afterCast->id] = ptr;
	if (afterCast->unique && hasComponent)
		RaiseError(Tools::TypeName<ComponentType>(), " is marked as unique, but there is already a ", Tools::TypeName<ComponentType>(), " at ", *this);
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
	ComponentType* afterCast = dynamic_cast<ComponentType*>(componentByID.at(id_));
	if (!afterCast)
		RaiseError("Failed to find " + Tools::TypeName<ComponentType>()
			+ " with id " + logger::to_string(id_));
	return *afterCast;
}

template <typename ComponentType>
ComponentType* Entity::TryGetComponent(uuids::uuid id_)
{
	if (!Tools::ContainsKey(componentByID, id_))
		return nullptr;
	ComponentType* afterCast = dynamic_cast<ComponentType*>(componentByID.at(id_));
	return afterCast;
}


template <typename... Args> 
Entity& Entity::Make(std::string name)
{
	Entity& entity = register_.Add(name);
	(..., entity.AddComponent<Args>());
	return entity;
}

template <typename ComponentType> 
void Entity::DeclareComp()
{
	//std::function<void(uuid)> func = [](uuid entityID) { Entity::Add<ComponentType>(entityID); };
	std::function<void(uuid, YAML::Node*)> func = [](uuid entityID, YAML::Node* node) {
		Entity::GetEntity(entityID).AddComponent<ComponentType>(node); };
	
	Entity::AddComponentByName[Tools::TypeName<ComponentType>()] = func;
};
