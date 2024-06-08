#pragma once
#include "Audio.h"

class Sound
{
public:
	Shorts;
	Sound(const string& filePath, bool hasPosition);
	~Sound();

	void PlayCopy(optional<vec2> position, float fadeDuration = 0);
	void Play(optional<vec2> position, float fadeDuration = 0);
	void Stop(float fadeDuration = 0);

	// inline Sound& SetPosition(vec2 position_) { position = position_; return *this; }
	void SetVolume(float volume);
	void SetPitch(float pitch);
	void SetPan(float pan);
	void SetLooping(bool isLooping);
	float Duration();

private:
	ma_sound ma_sound_;
	string filePath;
};

