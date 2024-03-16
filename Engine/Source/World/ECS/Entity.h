#pragma once
#include <unordered_map>
#include <string>
#include <type_traits>
#include <memory>
#include <stduuid/uuid.h>
#include "Register.h"
#include "YAML.h"

class Component; // forward declaration
	

class Entity
{
public:
	static Register<Entity> register_;
	std::string name;

	static uuids::uuid Make(std::string name = "Entity");
	Entity(std::string name = "Entity", uuids::uuid* id = nullptr);
	static void Update();
	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return componentsByEntity.at(id); }

	void Destroy(); // destroys the entity at the end of the component update calls
	static void Entity::DestroyEverything(); // engine-only

	template <typename ComponentType> inline static ComponentType* TryGet(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Get(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Add(uuids::uuid entityID);
	template <typename ComponentType> ComponentType& LoadComponent(YAML::Node& node);
	template <typename ComponentType> static ComponentType& GetComponent(uuids::uuid componentID);
	template <typename ComponentType> static ComponentType* TryGetComponent(uuids::uuid componentID);

	template <typename ComponentType> inline ComponentType* TryGet() { return TryGetComponent<ComponentType>(); };
	template <typename ComponentType> inline ComponentType& Get() { return GetComponent<ComponentType>(); };
	template <typename ComponentType> inline ComponentType& Add() { return AddComponent<ComponentType>(); };
	template <typename ComponentType> bool Destroy() { return Destroy(TryGetComponent<ComponentType>()); }

	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }
	static uuids::uuid GetID(std::string name);
	inline uuids::uuid GetID() const { return id; }
	inline void SetID(uuids::uuid id_) { id = id_; }
	std::string to_string() const;
	std::string GetName() { return name; }

private:
	static std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> componentsByEntity;
	static std::unordered_map<std::string, std::vector<uuids::uuid>> EntitiesByName;
	static std::unordered_map<uuids::uuid, Component*> componentsByID;
	uuids::uuid id;
	bool isDoomed = false; // this is destined to be destroyed at the end of the component update calls

	template <typename ComponentType> ComponentType* TryGetComponent() const;
	template <typename ComponentType> inline ComponentType& GetComponent() const;
	template <typename ComponentType> ComponentType& AddComponent(YAML::Node* node = nullptr); // id is only used by load
	void ClearData(); // Engine-only
	static void ClearData(const std::unique_ptr<Component>& compPtr); // Engine-only
	static void DestroyTheDoomed();
	bool Destroy(Component* comp); // destroys the entity at the end of the component update calls

};


#include "Entity_impl.h"
