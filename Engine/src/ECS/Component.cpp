#include "Component.h"
#include "Transform.h"
#include "UuidCreator.h"


void Component::InternalConstructor(uuids::uuid entityID_, YAML::Node* node)
{
	id = node == nullptr ? UuidCreator::MakeID() : (*node)["id"].as<uuids::uuid>();
	entityID = entityID_;
	transform = &Get<Transform>();
	if (node)
		Load(*node);
	OnConstructed(); // evt. combine Load and OnConstructed into Load with nullable pointer
}

Component::~Component()
{
	// is entity is null, then this components internal conctructor haven't 
	// been called yet in which case it doesn't count as having been fully created
	if (!UuidCreator::IsInitialized(entityID))
		return;
	//Log("~Component");
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



