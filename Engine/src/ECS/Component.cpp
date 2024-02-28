#include "Component.h"
#include "Transform.h"
#include "UuidCreator.h"

//std::unordered_map<uuids::uuid, Component*> Component::componentsByID;


void Component::OnAddComponent(uuids::uuid entityID_, YAML::Node* node_)
{
	if (node_)
	{
		node = *node_;
		id = node["id"].as<uuids::uuid>();
		entityID = entityID_;
		//componentsByID[id] = this;
	}
	else
	{
		id = UuidCreator::MakeID();
		entityID = entityID_;
		//componentsByID[id] = this;
		transform = &Get<Transform>();
		OnStart();
	}
}

void Component::OnSceneLoaded()
{
	transform = &Get<Transform>();
	Load(node);
	OnStart(); // evt. combine Load and OnConstructed into Load with nullable pointer
}
/*
void Component::Destroy()
{
	// is entity is null, then this components internal conctructor haven't 
	// been called yet in which case it doesn't count as having been fully created
	if (!UuidCreator::IsInitialized(entityID))
		return;
	OnDestroyed();
	Log("~Component: 1");
	auto hip = componentsByID.at(id);
	Log("~Component: 2");
	auto hi = componentsByID[id];
	Log("~Component: 3");
	auto iterator = componentsByID.find(id);
	Log("~Component: 4");
	Tools::RemoveKey_unordered(componentsByID, id); // this is buggy !!!
	Log("~Component: 5");

	// If the entity is dead, then it will handle cleanup itself.
	//if (!entity still exists)
	//	Tools::Remove(GetEntity().GetComponents(), this);   //<------------------ temp fix
}*/

std::string Component::to_string() const
{
	if (transform == nullptr)
		return GetEntity().name;
	else
		return transform->GetPath();
}


