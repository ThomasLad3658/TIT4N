#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <vector>
#include "Sound.hpp"

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void update();
	Sound* createSound(const char* filePath);
	Sound* createSound(SDL_AudioDeviceID device, const char* filePath);
private:
	std::vector<Sound> sounds;
};

