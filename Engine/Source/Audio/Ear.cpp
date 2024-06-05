#include "Ear.h"
#include "Audio.h"
#include "Transform.h"

Shorts;
const uint listenerIndex = 0; // we only have one listener

void Ear::OnStart()
{
    ma_engine_listener_set_world_up(&Audio::Engine(), listenerIndex, 0, 1, 0);
}

void Ear::OnUpdate()
{
    vec2 pos = GetTransform().Position2D();
    vec2 forward = GetTransform().Forward2D();

    ma_engine_listener_set_position(&Audio::Engine(), listenerIndex, pos.x, pos.y, 0);
    ma_engine_listener_set_direction(&Audio::Engine(), listenerIndex, forward.x, forward.y, 0);
}

void Ear::SetCone(float innerAngle, float outerAngle, float reductionFactor)
{
    // Makes sound volume go from fullVolume inside innerAngle to reductionFactor * fullVolume outside outerAngle.
    ma_engine_listener_set_cone(&Audio::Engine(), listenerIndex, innerAngle, outerAngle, reductionFactor);
}

