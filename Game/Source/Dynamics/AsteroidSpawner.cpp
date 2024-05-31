#include "AsteroidSpawner.h"
#include "Time_.h"
#include "StoredEntity.h"
#include "Transform.h"
#include "GlmTools.h"
#include "Player.h"

static std::string asteroidName = "asteroid_2";
static float minDuration = 0.0f;
static float maxDuration = 3.0f;
static float spawnDistance = 5.0f; // must be larger than viewport-half-width and viewport-half-height

static float nextSpawnTime = 0.0f;
static std::random_device rd;  // Non-deterministic generator
static std::mt19937 gen(rd()); // Mersenne Twister engine seeded with rd()
static std::uniform_real_distribution<float> timeDistribution(minDuration, maxDuration);
static std::uniform_real_distribution<float> angleDistribution(0, 2 * glm::PI);



void AsteroidSpawner::OnUpdate()
{
    if (Time::Now() < nextSpawnTime)
        return;

    nextSpawnTime += timeDistribution(gen);
    glm::vec2 spawnPosition = Player::transform().Position2D() + glm::PolarVec2(spawnDistance, angleDistribution(gen));

    StoredEntity::Load(asteroidName)
        .Get<Transform>().SetPosition2D(spawnPosition);

}


