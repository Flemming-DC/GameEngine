#include "Impact.h"
#include "RocketEngine.h"
#include "Delay.h"
#include "Player.h"
#include "StoredEntity.h"
Shorts;

static uint funcID;
static std::string rockName = "rock";

void Impact::OnStart()
{
	funcID = Get<Collider>().onEnter.Add([this](Collider& other) { Kill(other); });
}

void Impact::OnDestroy()
{
	Get<Collider>().onEnter.Remove(funcID);
}


void Impact::Kill(Collider& other)
{
	if (other == Player::collider())
		Player::rocketEngine().Die();


	// make dust
	//StoredEntity::Load(rockName);
	
	this->entity().Destroy();

}
