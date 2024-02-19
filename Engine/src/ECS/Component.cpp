#include "Component.h"
#include "Transform.h"
#include "UuidCreator.h"


void Component::InternalConstructor(uuids::uuid entityID_)
{
	id = UuidCreator::MakeID();
	entityID = entityID_;
	transform = &Get<Transform>();
	OnConstructed();
}

Component::~Component()
{
	// is entity is null, then this components internal conctructor haven't 
	// been called yet in which case it doesn't count as having been fully created
	if (!UuidCreator::IsInitialized(entityID))
		return;
	Log("~Component");
	OnDestroyed();
	// If the entity is dead, then it will handle cleanup itself.
	//if (!entityIsDoingcleanup)
	//	Tools::Remove(GetEntity().GetComponents(), this);   //<------------------ temp fix
}

std::string Component::to_string() const
{
	if (transform == nullptr)
		return GetEntity().name;
	else
		return transform->GetPath();
}



