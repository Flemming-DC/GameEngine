#pragma once
#include <unordered_map>
#include <string>
#include <type_traits>
#include <memory>
#include <stduuid/uuid.h>

// this pseudo entity component system doesn't handle memory layout in a contigous way
class Component;
	

class Entity
{
public:
	std::string name;

	Entity(std::string name = "Entity");
	~Entity();
	static void UpdateAllEntities(); 
	std::vector<Component*>& GetComponents() const { return componentsByID.at(this); }

	template <typename ComponentType> ComponentType* GetComponent() const;
	template <typename ComponentType> ComponentType* AddComponent();

	// evt. bool TryGetComponent(Component* component) or HasComponent
	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }


private:
	static std::unordered_map<const Entity*, std::vector<Component*>> componentsByID;
	uuids::uuid id;
};


#include "Entity_impl.h"
