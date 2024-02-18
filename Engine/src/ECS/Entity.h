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
	/*
	Entity(const Entity& other) : name(other.name), id(other.id) {} // Copy constructor
	Entity& operator=(const Entity& other)
	{ // Assignment operator
		if (this != &other)
		{
			name = other.name;
			id = other.id;
		}
		return *this;
	}*/
	void Destroy();
	//~Entity();
	static void UpdateAllEntities(); 
	std::vector<Component*>& GetComponents() const { return componentsByID.at(id); }


	static uuids::uuid Make(std::string name = "Entity");
	template <typename ComponentType> static ComponentType* Get(uuids::uuid entityID);
	template <typename ComponentType> static ComponentType* Add(uuids::uuid entityID);


	template <typename ComponentType> ComponentType* GetComponent() const;
	template <typename ComponentType> ComponentType* AddComponent();

	// evt. bool TryGetComponent(Component* component) or HasComponent
	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }
	//static Entity& GetEntity(uuids::uuid id) { return entitiesByID[id]; }
	uuids::uuid GetID() const { return id; }

//private:
	//static std::unordered_map<uuids::uuid, Entity&> entitiesByID;
	//static std::unordered_map<const Entity*, std::vector<Component*>> componentsByID; // this is not by id !!
	static std::unordered_map<uuids::uuid, std::vector<Component*>> componentsByID; // this is not by id !!
	uuids::uuid id;
};


#include "Entity_impl.h"
