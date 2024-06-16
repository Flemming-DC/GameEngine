#include "RocketEngine.h"
#include "GameInputs.h"
#include "Transform.h"
#include "Time_.h"
#include "GlmTools.h"
#include "GlmCheck.h"
#include "GameLiterals.h"
#include "GameAssets.h"
#include "Transform.h"
#include "Scene.h"
#include "Sound.h"

Shorts;
static bool isIgnited = false;
static const float speed = 1.0f;
static const float turnDuration = 0.5f;
static float exhaustFadingspeed = 0.2f;
// for non-singletons, put these on the instance.
static Material* exhaustMaterial = nullptr;
static Sound explosionSound(Literals::Sounds + "bum.wav", true);

void RocketEngine::OnStart()
{
	Assert(Entity::Exists(Entity::GetID("Exhaust")),
		"Exhaust not found.");

	exhaustMaterial = &Entity::GetEntity("Exhaust").Get<Renderable>().GetMaterial();
	exhaustMaterial->SetColor(vec4(1.0f, 1.0f, 1.0f, 0.0f));
	explosionSound.SetVolume(0.3f);
}


void RocketEngine::OnUpdate() // SetFlames, Rotate, Move
{
	if (GameInputs::Move().IsPressed() != isIgnited)
		isIgnited = !isIgnited;

	if (isIgnited)
	{
		vec2 targetDirection = GameInputs::Move().State();
		GetTransform().SmoothAngle(glm::Angle(targetDirection), turnDuration);

		vec2 currentDirection = GetTransform().Forward2D();
		GetTransform().IncrementPosition2D(speed * currentDirection * Time::Delta());
	}
	
	if (isIgnited)
		exhaustMaterial->IncrementColor(vec4(0.0f, 0.0f, 0.0f, exhaustFadingspeed));
	else
		exhaustMaterial->IncrementColor(vec4(0.0f, 0.0f, 0.0f, -exhaustFadingspeed));
}


void RocketEngine::Die()
{
	if (!Enabled())
		return;

	float deathDuration = 1;
	SetEnabled(false);

	exhaustMaterial->SetColor(vec4(1.0f, 1.0f, 1.0f, 0.0f));
	Get<Renderable>().GetMaterial().SetTexture(Literals::u_textureSampler, GameAssets::Explosion().GetID());
	explosionSound.Play(GetTransform().Position2D());

	Delay::ForSeconds(deathDuration, []() { Scene::Reload(); });

}
