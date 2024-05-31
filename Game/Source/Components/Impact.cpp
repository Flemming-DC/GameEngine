#include "Impact.h"
#include "RocketEngine.h"
#include "Delay.h"
#include "Player.h"
#include "StoredEntity.h"
Shorts;

static string rockName = "rock";

void Impact::OnStart()
{
	isRock = (entity().GetStoredID() == StoredEntity::naming.at(rockName));
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
	
	Impact* otherImpact = other.TryGet<Impact>();
	bool otherIsRock = otherImpact && otherImpact->IsRock();
	bool heavyObjectHittingRock = !isRock && otherIsRock;
	if (heavyObjectHittingRock)
		return;

	if (!isRock)
	{
		vec2 here = GetTransform().Position2D();
		StoredEntity::Load(rockName).Get<Transform>().SetPosition2D(here + vec2(0.0f, 0.2f));
		StoredEntity::Load(rockName).Get<Transform>().SetPosition2D(here + vec2(0.2f, 0.0f));
		StoredEntity::Load(rockName).Get<Transform>().SetPosition2D(here + vec2(-0.1f, -0.1f));
	}
	this->entity().Destroy();

}
