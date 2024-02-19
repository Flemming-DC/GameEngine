#include "Component.h"
#include "Transform.h"
#include "UuidCreator.h"

Register<Component> Component::register_;
//std::unordered_map<std::type_info, Register<Component>> Component::registers;

void Component::InternalConstructor(uuids::uuid entityID_)
{
	id = UuidCreator::MakeID();
	entityID = entityID_;
	transform = &Get<Transform>();
	OnConstructed();
	//Transform t = register_.AddCopy(Transform t);
}

Component::~Component()
{
	// is entity is null, then this components internal conctructor haven't 
	// been called yet in which case it doesn't count as having been fully created
	if (!UuidCreator::IsInitialized(entityID))
		return;
	//Log("Component destroyed, while entity is not null");
	OnDestroyed();
	// If the entity is dead, then it will handle cleanup itself.
	//if (!entityIsDoingcleanup)
	//	Tools::Remove(GetEntity().GetComponents(), this);   //<------------------ temp fix
}

std::string Component::to_string() const
{
	//*
	//Transform* transform = GetTransform();// GetEntity().GetComponent<Transform>();
	if (transform == nullptr)
		return GetEntity().name;// +"." + Tools::to_string(*this);
	else
		return transform->GetPath();// + "." + Tools::to_string(*this);
	//*/
}



