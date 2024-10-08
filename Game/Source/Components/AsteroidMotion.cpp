#include "AsteroidMotion.h"
#include "Transform.h"
#include "Time_.h"
#include "Player.h"
#include "GlmCheck.h"


static float dispersion = 0.5f;
//static float speed = 1.0f;  // make editable
//static float angularSpeed = 0.5f; 

static std::random_device rd;  // Non-deterministic generator
static std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()
static std::uniform_real_distribution<float> angleDistribution(-dispersion, dispersion);



void AsteroidMotion::OnUpdate()
{
	if (glm::LessThan(velocity, GlmCheck::realisticallySmall))
	{
		glm::vec2 fromHereToPlayer = Player::transform().Position2D() - GetTransform().Position2D();
		float angle = glm::Angle(fromHereToPlayer) + angleDistribution(gen);
		velocity = glm::PolarVec2(speed, angle);
	}

	GetTransform().IncrementPosition2D(velocity * Time::Delta());
	GetTransform().IncrementAngle(angularSpeed * Time::Delta());
}


void AsteroidMotion::Save(YAML::Node& node) const
{
	node["speed"] = speed;
	//node["dispersion"] = dispersion;
	node["angularSpeed"] = angularSpeed;
}

void AsteroidMotion::Load(const YAML::Node& node)
{
	speed = node["speed"].as<float>();
	//dispersion = node["dispersion"].as<float>();
	angularSpeed = node["angularSpeed"].as<float>();
}


