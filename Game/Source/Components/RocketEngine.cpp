#include "RocketEngine.h"
#include "GameInputs.h"
#include "Transform.h"
#include "Time_.h"
#include "GlmTools.h"
#include "GlmCheck.h"
#include <glm/gtc/constants.hpp>

Shorts;
const float speed = 1.0f;
const float angularSpeed = 10.0f;

void RocketEngine::OnStart()
{

}


void RocketEngine::OnUpdate()
{
	vec2 velocity = speed * GameInputs::Move().State();
	GetTransform().IncrementPosition2D(velocity * Time::Delta());

	if (glm::LessThan(velocity, GlmCheck::realisticallySmall))
		return;

	float targetAngle = glm::atan(velocity.y, velocity.x);
	float currentAngle = GetTransform().Angle();
	float step = angularSpeed * Time::Delta();

	float nextAngle = glm::SmoothAngle(currentAngle, targetAngle, step);
	GetTransform().SetAngle(nextAngle);

}


