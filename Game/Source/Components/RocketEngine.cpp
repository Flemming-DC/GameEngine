#include "RocketEngine.h"
#include "GameInputs.h"
#include "Transform.h"
#include "Time_.h"
#include "GlmTools.h"
#include "GlmCheck.h"
#include "GameLiterals.h"
#include "GameAssets.h"

Shorts;
const float speed = 1.0f;
const float angularSpeed = 10.0f;
bool isIgnited = false;

void RocketEngine::OnStart()
{
	material = &Get<Renderable>().GetMaterial();

}


void RocketEngine::OnUpdate() // SetFlames, Rotate, Move
{
	if (GameInputs::Move().IsPressed() != isIgnited)
	{
		isIgnited = !isIgnited;
		Texture& tex = isIgnited ? GameAssets::RocketFlamingTex() : GameAssets::RocketTex();
		material->SetTexture(Literals::u_textureSampler, tex.GetID());
	}


	if (isIgnited)
	{
		vec2 targetDirection = GameInputs::Move().State();
		GetTransform().SmoothAngle(glm::Angle(targetDirection), angularSpeed * Time::Delta());

		vec2 currentDirection = GetTransform().Forward2D();
		GetTransform().IncrementPosition2D(speed * currentDirection * Time::Delta());
	}


}


