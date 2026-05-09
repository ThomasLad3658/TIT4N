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
	int createSound(const std::string& filePath);
	int createSound(const std::string& filePath, SDL_AudioDeviceID dev);
	bool exists(int soundId);
	void play(int soundId);
	bool isPlaying(int soundId);
	void stop(int soundId);
	void resume(int soundId);
	void pause(int soundId);
	void loop(int soundId, bool loop);
private:
	bool registerSound(std::unique_ptr<Sound> sound);
	bool unregisterSound(int soundId);
	std::vector<std::unique_ptr<Sound>> sounds;
};

