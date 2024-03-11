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
	const std::string name; 

	static uuids::uuid Make(std::string name = "Entity");
	Entity(std::string name = "Entity", uuids::uuid* id = nullptr);
	void Destroy();
	static void UpdateAllEntities(); 
	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return componentsByEntity.at(id); }
	bool RemoveComponent(const Component& comp);
	
	template <typename ComponentType> inline static ComponentType* TryGet(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Get(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Add(uuids::uuid entityID);
	template <typename ComponentType> ComponentType& LoadComponent(YAML::Node& node);
	template <typename ComponentType> static ComponentType& GetComponent(uuids::uuid componentID);
	template <typename ComponentType> static ComponentType* TryGetComponent(uuids::uuid componentID);


	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }
	static uuids::uuid GetID(std::string name);
	inline uuids::uuid GetID() const { return id; }
	inline void SetID(uuids::uuid id_) { id = id_; }
	std::string to_string() const;

private:
	static std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> componentsByEntity;
	static std::unordered_map<std::string, std::vector<uuids::uuid>> EntitiesByName;
	static std::unordered_map<uuids::uuid, Component*> componentsByID;
	uuids::uuid id;
	bool isDestroyed = false; // this means that destroy has been called

	template <typename ComponentType> ComponentType* TryGetComponent() const;
	template <typename ComponentType> inline ComponentType& GetComponent() const;
	template <typename ComponentType> ComponentType& AddComponent(YAML::Node* node = nullptr); // id is only used by load
};


#include "Entity_impl.h"
