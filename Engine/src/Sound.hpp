#pragma once
#include <SDL3/SDL.h>
#include <iostream>

enum SoundErrorId {
	LOAD_WAV,
	LOAD_DEVICE
};

class Sound
{
public:
	Sound(std::string filePath, SDL_AudioDeviceID dev);
	~Sound();
	int getErrorId() const;
	int getStartTick() const;
	bool isUsable() const;
	bool isPlaying() const;
	void play();
	void update();
	void stop();
	void pause();
	void resume();
	bool loop;
private:
	void cleanUp();
	void init(std::string filePath);
	SDL_AudioSpec spec;
	SDL_AudioSpec devSpec;
	Uint8* buffer;
	Uint32 length;
	SDL_AudioStream* stream;
	SDL_AudioDeviceID device;
	int startTick;
	bool playing;
	bool paused;
	bool usable;
	int errorId;
	float duration;
};