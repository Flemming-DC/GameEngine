#pragma once
#include <string>
#include "Entity.h"
#include "Event.h"
#include <stduuid/uuid.h>
#include "Register.h"
#include <unordered_map>
#include <typeinfo>

class Transform;
class Entity;

// evt. make updatable a subclass of component to avoid updating to many
// evt. add an extra index for dynamics / updatables, which indicates there component type
class Component
{
	// entity must access InternalConstructor and OnUpdate and it must notify component about entity being dead
	friend class Entity; 
public:
	~Component();
	std::string to_string() const;

	template <typename ComponentType> inline 
		ComponentType* TryGet() const { return Entity::TryGet<ComponentType>(entityID); }
	template <typename ComponentType> inline 
		ComponentType& Get() const { return Entity::Get<ComponentType>(entityID); }
	inline Entity& GetEntity() const { return Entity::register_.Get(entityID); } //{ return entity; }
	inline Transform& GetTransform() const { return *transform; }

	bool operator==(const Component& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Component& other) { return this->id == other.id; }
	inline uuids::uuid GetID() const { return id; }
	virtual void Save(YAML::Node& node) const {}

protected:
	Component() {}; // called by Entity
	virtual void Load(const YAML::Node& node) {}

private:
	uuids::uuid entityID;
	Transform* transform = nullptr;
	//bool entityIsDoingcleanup = false;
	uuids::uuid id;
	//static std::unordered_map<uuids::uuid, Component*> componentsByID;

	void InternalConstructor(uuids::uuid entityID_, YAML::Node* node = nullptr); // called by Entity

	virtual void OnConstructed() {}
	virtual void OnDestroyed() {}
	virtual void OnUpdate() {}


};



