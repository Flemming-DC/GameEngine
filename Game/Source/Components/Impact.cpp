#include "Impact.h"
#include "RocketEngine.h"
#include "Delay.h"
#include "Scene.h"
#include "Player.h"
Shorts;

static uint funcID;

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
	// missing death anim
	if (other == Player::collider())
	{
		Scene::Reload();
	}
	else
	{
		other.Entity().Destroy();
		this->Entity().Destroy();
	}

}
