#include "Player.h"
#include "Audio.h" // temp

static Transform* transformPtr = nullptr;
static Collider* colliderPtr = nullptr;
static RocketEngine* rocketEnginePtr = nullptr;


void Player::OnStart()
{
	// instance should not already be initialized, nor do we allow scramble data from previous playthroughs
	Deny(transformPtr || colliderPtr || rocketEnginePtr,
		"The player singleton already exists.");
	//assert(!transformPtr && !colliderPtr && !rocketEnginePtr); 
	transformPtr = &GetTransform();
	colliderPtr = &Get<Collider>();
	rocketEnginePtr = &Get<RocketEngine>();
}

void Player::OnDestroy()
{
	// this prevents scramble data
	transformPtr = nullptr;
	colliderPtr = nullptr;
	rocketEnginePtr = nullptr;
}

void Player::OnUpdate()
{

}

Collider& Player::collider()
{
	assert(colliderPtr); // Cannot get instance, before it is initialized.
	return *colliderPtr;
}

Transform& Player::transform()
{
	assert(transformPtr); // Cannot get instance, before it is initialized.
	return *transformPtr;
}

RocketEngine& Player::rocketEngine()
{
	assert(rocketEnginePtr); // Cannot get instance, before it is initialized.
	return *rocketEnginePtr;
}


