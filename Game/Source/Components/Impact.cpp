#include "Impact.h"
#include "RocketEngine.h"
#include "Delay.h"
#include "Player.h"
#include "StoredEntity.h"
#include "GameLiterals.h"
#include "GameAssets.h"
#include "Sound.h"
Shorts;

static string rockName = "rock";
static Sound impactSound(Literals::Sounds + "dummy.wav", true);

void Impact::OnStart()
{
	isRock = (entity().GetStoredID() == StoredEntity::naming.at(rockName));
	funcID = Get<Collider>().onEnter.Add([this](Collider& other) { OnColliderEnter(other); });
	//impactSound = &Sound(Literals::Sounds + "dummy.wav", true);
}

void Impact::OnDestroy()
{
	Get<Collider>().onEnter.Remove(funcID);
}


void Impact::OnColliderEnter(Collider& other)
{
	Impact* otherImpact = other.TryGet<Impact>();
	bool otherIsRock = otherImpact && otherImpact->IsRock();
	bool heavyObjectHittingRock = !isRock && otherIsRock;
	if (heavyObjectHittingRock)
		return;

	if (other == Player::collider())
		Player::rocketEngine().Die();

	if (!isRock)
	{
		vec2 here = GetTransform().Position2D();
		StoredEntity::Load(rockName).Get<Transform>().SetPosition2D(here + vec2(0.0f, 0.2f));
		StoredEntity::Load(rockName).Get<Transform>().SetPosition2D(here + vec2(0.2f, 0.0f));
		StoredEntity::Load(rockName).Get<Transform>().SetPosition2D(here + vec2(-0.1f, -0.1f));
	}

	float deathDuration = 0.65f;
	Get<Collider>().SetEnabled(false);
	Get<Renderable>().GetMaterial().SetTexture(Literals::u_textureSampler, GameAssets::Dust().GetID());
	//if (!impactSound)
	//	RaiseError("impactSound was nullptr");
	impactSound.Start(GetTransform().Position2D());

	Delay::ForSeconds(deathDuration, [this]() { this->entity().Destroy(); });

}
