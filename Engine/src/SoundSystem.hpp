#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "Sound.hpp"
#include <memory>

class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	void update();
	int createSound(std::string filePath);
	int createSoundWithDev(std::string filePath, SDL_AudioDeviceID dev);
	bool exists(int soundId);
	bool destroySound(int soundId);
	void play(int soundId);
	void playFrom(int soundId, int startMs);
	void playFromTo(int soundId, int startMs, int endMs);
	bool isPlaying(int soundId);
	void stop(int soundId);
	void resume(int soundId);
	void pause(int soundId);
	void loop(int soundId, bool loop);
	Uint64 getStartTick(int soundId);
	Uint64 getEndTick(int soundId);
private:
	bool unregisterSound(int soundId);
	std::vector<std::unique_ptr<Sound>> sounds;
};

