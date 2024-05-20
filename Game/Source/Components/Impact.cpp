#include "Impact.h"
#include "RocketEngine.h"
#include "Delay.h"
#include "Scene.h"


void Impact::OnStart()
{
	Get<Collider>().onEnter.Add([this](Collider& other) { Kill(other); });
}


void Impact::Kill(Collider& other)
{
	// missing death anim
	if (other.TryGet<RocketEngine>())
	{
		Scene::ReloadImmediately();
	}
	else
	{
		other.Entity().Destroy();
		this->Entity().Destroy();
	}

}
