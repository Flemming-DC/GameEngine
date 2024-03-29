#include "Component.h"
#include "Transform.h"
#include "UuidCreator.h"


void Component::OnAddComponent(uuids::uuid entityID_, YAML::Node* node_)
{
	if (node_)
	{
		node = *node_;
		id = node["id"].as<uuids::uuid>();
		entityID = entityID_;
	}
	else
	{
		id = UuidCreator::MakeID();
		entityID = entityID_;
		transform = TryGet<Transform>();
		if (!transform)
			transform = &Entity::Add<Transform>(entityID);
		OnStart();
	}

}

void Component::OnSceneLoaded()
{
	transform = &Get<Transform>();
	Load(node);
	OnStart(); // evt. combine Load and OnConstructed into Load with nullable pointer
}

std::string Component::to_string() const
{
	if (transform && UuidCreator::IsInitialized(entityID))
		return transform->GetPath() + "." + Tools::TypeName(*this);
	else if (UuidCreator::IsInitialized(entityID))
		return "uninitialized " + GetEntity().GetName() + "." + Tools::TypeName(*this);
	else
		return "uninitialized " + Tools::TypeName(*this);
}


