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
#include "Audio.h"
#include "Sound.h"

Shorts;
static bool isIgnited = false;
static const float speed = 1.0f;
static const float turnDuration = 0.5f;
static float exhaustFadingspeed = 0.2f;
// for non-singletons, put these on the instance.
static Material* exhaustMaterial = nullptr;
static Sound explosionSound(Literals::Sounds + "dummy.wav", true);

void RocketEngine::OnStart()
{
	if (!Entity::Exists(Entity::GetID("Exhaust")))
		RaiseError("Exhaust not found.");

	exhaustMaterial = &Entity::GetEntity("Exhaust").Get<Renderable>().GetMaterial();
	exhaustMaterial->SetColor(vec4(1.0f, 1.0f, 1.0f, 0.0f));

}


void RocketEngine::OnUpdate() // SetFlames, Rotate, Move
{
	if		(InputKey::BecomesPressed(Key::Keyboard::_1)) explosionSound.Start(vec2(std::pow(2, 1), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_2)) explosionSound.Start(vec2(std::pow(2, 2), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_3)) explosionSound.Start(vec2(std::pow(2, 3), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_4)) explosionSound.Start(vec2(std::pow(2, 4), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_5)) explosionSound.Start(vec2(std::pow(2, 5), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_6)) explosionSound.Start(vec2(std::pow(2, 6), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_7)) explosionSound.Start(vec2(std::pow(2, 7), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_8)) explosionSound.Start(vec2(std::pow(2, 8), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_9)) explosionSound.Start(vec2(std::pow(2, 9), 0));
	else if (InputKey::BecomesPressed(Key::Keyboard::_0)) explosionSound.Start(vec2(std::pow(2, 10),0));

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
	explosionSound.Start(GetTransform().Position2D());

	Delay::ForSeconds(deathDuration, []() { Scene::Reload(); });

}
