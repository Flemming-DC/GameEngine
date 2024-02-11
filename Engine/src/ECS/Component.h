#pragma once
#include <string>
#include "Entity.h"
#include "Event.h"
#include <stduuid/uuid.h>

class Transform;
class Entity;

// evt. make updatable a subclass of component to avoid updating to many
// evt. add an extra index for dynamics / updatables, which indicates there component type
class Component
{
	// entity must access InternalConstructor and OnUpdate and it must notify component about entity being dead
	friend class Entity; 
public:

	Component() { Log("Component created"); };
	~Component();
	std::string to_string() const;

	template <typename ComponentType>
	inline ComponentType* Get() const { return entity->GetComponent<ComponentType>(); }
	inline Entity* GetEntity() const { return entity; }
	inline Transform* GetTransform() const { return transform; }


	bool operator==(const Component& other) { return this->has_equal_id(other); }
	bool has_equal_id(const Component& other) { return this->id == other.id; }

private:
	Entity* entity = nullptr; 
	Transform* transform = nullptr;
	bool entityIsDoingcleanup = false;
	uuids::uuid id;

	void InternalConstructor(Entity* entity); // only called by AddComponent in Entity

	virtual void OnConstructed() {}
	virtual void OnDestroyed() {}
	virtual void OnUpdate() {}


};



