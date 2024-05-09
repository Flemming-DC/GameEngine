#pragma once
#include <unordered_map>
#include <string>
#include <type_traits>
#include <memory>
#include <stduuid/uuid.h>
#include "Register.h"
#include "YmlTools.h"
#include "Event.h"

class Component; // forward declaration

class Entity
{ 
public: 
	Shorts;
	static Register<Entity> register_;
	static Event<Entity&> OnCreated;
	static Event<Entity&> OnDestroy;
	static map_uo<string, std::function<void(uuid, YAML::Node*)>> AddComponentByName; // rename and evt. hide

	Entity(string name = "Entity", optional<uuid> id_ = std::nullopt, optional<uuid> storedID_ = std::nullopt); // id bruges hvis entiteten loades fra disk
	static void Update();
	const vector<unique_ptr<Component>>& GetComponents() const { return componentsByEntity.at(id); }

	void Destroy(); // destroys the entity at the end of the component update calls
	static void Entity::DestroyEverything(); // engine-only
	static void DestroyTheDoomed(); // engine-only

	template <typename ComponentType> inline static ComponentType* TryGet(uuid entityID);
	template <typename ComponentType> inline static ComponentType& Get(uuid entityID);
	template <typename ComponentType> inline static ComponentType& Add(uuid entityID);
	template <typename ComponentType> static ComponentType& GetComponent(uuid componentID);
	template <typename ComponentType> static ComponentType* TryGetComponent(uuid componentID);

	template <typename ComponentType> static void DeclareComp();
	template <typename... Args> static Entity& Make(string name = "Entity");
	template <typename... Args> static void DeclareComps() { (..., DeclareComp<Args>()); };
	template <typename... Args> void AddMultiple() { (..., AddComponent<Args>()); };
	template <typename ComponentType> inline ComponentType* TryGet() const { return TryGetComponent<ComponentType>(); };
	template <typename ComponentType> inline ComponentType& Get() const { return GetComponent<ComponentType>(); };
	template <typename ComponentType> inline ComponentType& Add() { return AddComponent<ComponentType>(); };
	template <typename ComponentType> bool Destroy() { return Destroy(GetComponent<ComponentType>()); }


	// evt. get component in children, parent, siblings etc.

	bool operator==(const Entity& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Entity& other) { return this->id == other.id; }
	static uuid GetID(string name);
	inline uuid GetID() const { return id; }
	inline optional<uuid> GetStoredID() const { return storedID; }
	static uuid* Entity::TryGetID(string name_);
	inline void SetID(uuid id_) { id = id_; }
	string to_string() const;
	string Name() const { return name; }
	static Entity& GetEntity(string name) { return register_.Get(GetID(name)); }
	static Entity& GetEntity(uuid id_) { return register_.Get(id_); }
	static bool Exists(uuid id_) { return register_.Contains(id_); }
	bool Destroy(Component& comp); // destroys the component at the end of the component update calls

private:
	static map_uo<uuid, vector<unique_ptr<Component>>> componentsByEntity;
	static map_uo<string, vector<uuid>> EntitiesByName;
	static map_uo<uuid, Component*> componentByID;
	uuid id;
	string name;
	bool isDoomed = false; // this is destined to be destroyed at the end of the component update calls
	optional<uuid> storedID = std::nullopt;

	template <typename ComponentType> ComponentType* TryGetComponent() const;
	template <typename ComponentType> inline ComponentType& GetComponent() const;
	template <typename ComponentType> ComponentType& AddComponent(YAML::Node* node = nullptr); // id is only used by load
	void ClearData(); // Engine-only
	static void ClearData(const unique_ptr<Component>& compPtr); // Engine-only
	static void CheckConsistency();

};

#include "Entity_impl.h"



