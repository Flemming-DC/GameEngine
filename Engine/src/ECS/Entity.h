#pragma once
#include <unordered_map>
#include <string>
#include <type_traits>
#include <memory>
#include <stduuid/uuid.h>
#include "Register.h"
#include "YAML.h"

// this pseudo entity component system doesn't handle memory layout in a contigous way
class Component;
	

class Entity
{
public:
	static Register<Entity> register_;
	const std::string name; 

	Entity(std::string name = "Entity", uuids::uuid* id = nullptr);
	void Destroy();
	//~Entity();
	static void UpdateAllEntities(); 
	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return componentsByEntity.at(id); }


	static uuids::uuid Make(std::string name = "Entity");
	template <typename ComponentType> inline static ComponentType* TryGet(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Get(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Add(uuids::uuid entityID);
	template <typename ComponentType> ComponentType& LoadComponent(YAML::Node& node);


	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }
	static uuids::uuid GetID(std::string name);
	inline uuids::uuid GetID() const { return id; }
	inline void SetID(uuids::uuid id_) { id = id_; }

private:
	static std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> componentsByEntity;
	static std::unordered_map<std::string, std::vector<uuids::uuid>> EntitiesByName;
	uuids::uuid id;

	template <typename ComponentType> ComponentType* TryGetComponent() const;
	template <typename ComponentType> inline ComponentType& GetComponent() const;
	template <typename ComponentType> ComponentType& AddComponent(YAML::Node* node = nullptr); // id is only used by load
};


#include "Entity_impl.h"
