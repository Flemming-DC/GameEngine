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

Shorts;
static bool isIgnited = false;
static const float speed = 1.0f;
static const float angularSpeed = 10.0f;
static float exhaustFadingspeed = 0.2f;
static Material* exhaustMaterial = nullptr;

void RocketEngine::OnStart()
{
	if (!Entity::Exists(Entity::GetID("Exhaust")))
		RaiseError("Exhaust not found.");

	//exhaustMaterial = &GetTransform().GetChildren()[0]->Get<Renderable>().GetMaterial();
	exhaustMaterial = &Entity::GetEntity("Exhaust").Get<Renderable>().GetMaterial();
	exhaustMaterial->SetColor(vec4(1.0f, 1.0f, 1.0f, 0.0f));
	//material = &Get<Renderable>().GetMaterial();
}


void RocketEngine::OnUpdate() // SetFlames, Rotate, Move
{
	if (GameInputs::Move().IsPressed() != isIgnited)
	{
		isIgnited = !isIgnited;
		//Texture& tex = isIgnited ? GameAssets::RocketFlamingTex() : GameAssets::RocketTex();
		//material->SetTexture(Literals::u_textureSampler, tex.GetID());
	}


	if (isIgnited)
	{
		vec2 targetDirection = GameInputs::Move().State();
		GetTransform().SmoothAngle(glm::Angle(targetDirection), angularSpeed * Time::Delta());

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
	float deathDuration = 1;
	SetEnabled(false);

	exhaustMaterial->SetColor(vec4(1.0f, 1.0f, 1.0f, 0.0f));
	Get<Renderable>().GetMaterial().SetTexture(Literals::u_textureSampler, GameAssets::ExplosionTex().GetID());

	Delay::ForSeconds(deathDuration, []() { Scene::Reload(); });

}
