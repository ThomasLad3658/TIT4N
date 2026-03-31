#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <vector>
class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void update();
	int playSound(const char* filePath);
	int playSound(SDL_AudioDeviceID device, const char* filePath);
private:
	std::vector<SDL_AudioStream*> streams;
};

