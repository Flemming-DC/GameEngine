#include "Sound.h"
#include "ErrorChecker.h"
#include "logger.h"
#include "GlmCheck.h"
#include "Delay.h"
#include <filesystem>
Shorts;
/*

ma_sound_is_playing(&sound)
evt. dont double load the same file - evt. use the Resource Management from miniaudio
*/

/*
defaults to ma_attenuation_model_inverse, wich works as follows:

                            min
gain =   ----------------------------------------   =  konst / distanceClamped, for distance >> min, with konst = min / rolloff
         min + rolloff * (distanceClamped - min)      1                       , for distance == min

and where min = minDistance and distanceClamped = clamp(distance, minDistance, maxDistance)
*/
static float rolloff        = 4.0f;     // default: 1
static float minGain        = 0.0f;     // default: 0
static float maxGain        = 1.0f;     // default: 1
static float minDistance    = 4.0f;     // default: 1
static float maxDistance    = FLT_MAX;  // default: FLT_MAX
static float dopplerFactor  = 1.0f;     // default: 1

Sound::Sound(const string& filePath, bool hasPosition)
    : ma_sound_(ma_sound())
{
    if (!std::filesystem::exists(filePath))
        RaiseError("Cannot play audioFile, since it does not exist. ", filePath);

    int flag = MA_SOUND_FLAG_DECODE; // decode sound on load, not every time its played, for performance reasons.
    if (!hasPosition)
        flag |= MA_SOUND_FLAG_NO_SPATIALIZATION;
    
    ma_result result = ma_sound_init_from_file(&Audio::Engine(), filePath.c_str(), flag, nullptr, nullptr, &ma_sound_);
    if (result != MA_SUCCESS)
        RaiseError("Failed to create sound. ", result);

    
    ma_sound_set_rolloff(&ma_sound_, rolloff);
    ma_sound_set_min_gain(&ma_sound_, minGain);
    ma_sound_set_max_gain(&ma_sound_, maxGain);
    ma_sound_set_min_distance(&ma_sound_, minDistance);
    ma_sound_set_max_distance(&ma_sound_, maxDistance);
    ma_sound_set_doppler_factor(&ma_sound_, dopplerFactor);
}

Sound::~Sound()
{
    ma_sound_uninit(&ma_sound_);
}

void Sound::Start(optional<vec2> position, float fadeDuration)
{
    if (position)
        ma_sound_set_position(&ma_sound_, position->x, position->y, 0.0f);

    if (fadeDuration > GlmCheck::realisticallySmall)
    {
        float volume = ma_sound_get_volume(&ma_sound_);
        ma_sound_set_fade_in_milliseconds(&ma_sound_, 0, volume, (ma_uint64)fadeDuration * 1000);
    }
    
    ma_sound_seek_to_pcm_frame(&ma_sound_, 0);
    ma_result result = ma_sound_start(&ma_sound_);
    if (result != MA_SUCCESS)
        Warning("Failed to play sound. ", result);
}


void Sound::Stop(float fadeDuration)
{
    if (fadeDuration > GlmCheck::realisticallySmall)
    {
        float volume = ma_sound_get_volume(&ma_sound_);
        ma_sound_set_fade_in_milliseconds(&ma_sound_, volume, 0, (ma_uint64)fadeDuration * 1000);
    }

    Delay::ForSeconds(fadeDuration, [&]() {
        ma_result result = ma_sound_stop(&ma_sound_);
        if (result != MA_SUCCESS)
            Warning("Failed to stop sound. ", result);
    });

}



void Sound::SetVolume(float volume)
{
    if (volume < -GlmCheck::realisticallySmall)
        RaiseError("The volume should be greater than 0. Received ", volume);
    ma_sound_set_volume(&ma_sound_, volume);
}

void Sound::SetPitch(float pitch)
{
    if (pitch < -GlmCheck::realisticallySmall)
        RaiseError("The pitch should be greater than 0. Received ", pitch);
    ma_sound_set_pitch(&ma_sound_, pitch);
}

void Sound::SetPan(float pan)
{
    if (std::abs(pan) > 1 + GlmCheck::realisticallySmall)
        RaiseError("The pan should be between -1 (pan left) and 1 (pan right). Received ", pan);
    ma_sound_set_pan(&ma_sound_, pan);
}

void Sound::SetLooping(bool isLooping)
{
    ma_sound_set_looping(&ma_sound_, isLooping);
}

float Sound::Duration()
{
    ma_uint64 pcnFrameCount;
    ma_uint32 pcmFrameRate; // ma_uint32 = uint

    ma_data_source* dataSource = ma_sound_get_data_source(&ma_sound_);
    ma_result result = ma_sound_get_data_format(&ma_sound_, nullptr, nullptr, &pcmFrameRate, nullptr, 0);
    if (result != MA_SUCCESS)
        Warning("Failed to get pcmFrameRate. ", result);
    result = ma_data_source_get_length_in_pcm_frames(dataSource, &pcnFrameCount);
    if (result != MA_SUCCESS)
        Warning("Failed to get pcnFrameCount. ", result);

    float duration = (float)pcnFrameCount / pcmFrameRate;
    return duration;
}

