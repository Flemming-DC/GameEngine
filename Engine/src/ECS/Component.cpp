#include "Component.h"
#include "Transform.h"
#include "UuidCreator.h"


void Component::InternalConstructor(Entity* entity_)
{
	id = UuidCreator::MakeUUID();
	entity = entity_;
	transform = Get<Transform>();
	OnConstructed();
}

Component::~Component()
{
	// is entity is null, then this component the internal conctructor haven't 
	// been called yet in which case it doesn't count as having been fully created
	if (!entity)
		return;
	Log("Component destroyed, while entity is not null");
	OnDestroyed();
	// If the entity is dead, then it wiil handle cleanup itself.
	if (!entityIsDoingcleanup)
		Tools::Remove(entity->GetComponents(), this);
}

std::string Component::to_string() const
{
	Transform* transform = entity->GetComponent<Transform>();
	if (transform == nullptr)
		return entity->name;
	else
		return transform->GetPath();
}



