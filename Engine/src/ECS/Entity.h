#pragma once
#include <unordered_map>
#include <string>
#include <type_traits>
#include <memory>
#include <stduuid/uuid.h>
#include "Register.h"

// this pseudo entity component system doesn't handle memory layout in a contigous way
class Component;
	

class Entity
{
public:
	static Register<Entity> register_;
	std::string name;

	Entity(std::string name = "Entity");
	void Destroy();
	//~Entity();
	static void UpdateAllEntities(); 
	const std::vector<std::unique_ptr<Component>>& GetComponents() const { return componentsByEntity.at(id); }


	static uuids::uuid Make(std::string name = "Entity");
	template <typename ComponentType> inline static ComponentType* TryGet(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Get(uuids::uuid entityID);
	template <typename ComponentType> inline static ComponentType& Add(uuids::uuid entityID);


	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }
	inline uuids::uuid GetID() const { return id; }

private:
	static std::unordered_map<uuids::uuid, std::vector<std::unique_ptr<Component>>> componentsByEntity;
	uuids::uuid id;

	template <typename ComponentType> ComponentType* TryGetComponent() const;
	template <typename ComponentType> inline ComponentType& GetComponent() const;
	template <typename ComponentType> ComponentType& AddComponent();
};


#include "Entity_impl.h"
