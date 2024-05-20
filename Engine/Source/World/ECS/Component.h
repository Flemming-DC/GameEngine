#pragma once
#include <string>
#include <stduuid/uuid.h>
#include <unordered_map>
#include <typeinfo>
#include "ErrorChecker.h"
#include "Entity.h"
#include "Event.h"
#include "Register.h"

class Transform;
class Entity;



// evt. make updatable a subclass of component to avoid updating to many
// evt. add an extra index for dynamics / updatables, which indicates there component type
class Component
{
	// entity must access InternalConstructor and OnUpdate and it must notify component about entity being dead
	friend class Entity; 
public:

	std::string to_string() const;

	template <typename ComponentType> inline 
		ComponentType* TryGet() const { return Entity::TryGet<ComponentType>(entityID); }
	template <typename ComponentType> inline 
		ComponentType& Get() const { return Entity::Get<ComponentType>(entityID); }
	inline Entity& Entity() const { return Entity::GetEntity(entityID); } //{ return entity; }
	inline Transform& GetTransform() const { return *transform; };

	bool operator==(const Component& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Component& other) { return this->id == other.id; }
	inline uuids::uuid GetID() const { return id; }
	virtual void Save(YAML::Node& node) const {}
	void OnSceneLoaded();
	virtual int InitOrder() { return 0; }

protected:
	bool unique = false; // if unique, then the entity can only contain a single component of this type.

	Component() {}; // called by Entity
	virtual void Load(const YAML::Node& node) {}


private:
	uuids::uuid entityID;
	Transform* transform = nullptr;
	uuids::uuid id;
	YAML::Node node; // This is the node from which the component was loaded, if it was loaded at all
	bool isDoomed = false; // accessed from Entity

	void OnAddComponent(uuids::uuid entityID_, YAML::Node* node = nullptr); // called by Entity

	inline virtual void OnStart() {} // called when the component is created, activated and loaded, whichever happens last.
	inline virtual void OnDestroy() {}
	inline virtual void OnUpdate() {}


};
